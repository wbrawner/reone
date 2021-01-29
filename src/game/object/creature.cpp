/*
 * Copyright (c) 2020-2021 The reone project contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "creature.h"

#include <climits>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "../../common/log.h"
#include "../../common/streamutil.h"
#include "../../common/timer.h"
#include "../../net/types.h"
#include "../../render/models.h"
#include "../../render/textures.h"
#include "../../resource/resources.h"
#include "../../scene/types.h"
#include "../../script/types.h"

#include "../action/attack.h"
#include "../blueprint/blueprints.h"
#include "../portraitutil.h"

#include "objectfactory.h"

using namespace std;

using namespace reone::net;
using namespace reone::render;
using namespace reone::resource;
using namespace reone::scene;
using namespace reone::script;

namespace reone {

namespace game {

static constexpr int kStrRefRemains = 38151;

static string g_headHookNode("headhook");
static string g_talkDummyNode("talkdummy");

Creature::Creature(
    uint32_t id,
    ObjectFactory *objectFactory,
    SceneGraph *sceneGraph,
    ScriptRunner *scriptRunner
) :
    SpatialObject(id, ObjectType::Creature, objectFactory, sceneGraph, scriptRunner),
    _modelBuilder(this),
    _animResolver(this) {

    _drawDistance = 2048.0f;
}

bool Creature::isSelectable() const {
    bool hasDropableItems = false;
    for (auto &item : _items) {
        if (item->isDropable()) {
            hasDropableItems = true;
            break;
        }
    }
    return !_dead || hasDropableItems;
}

void Creature::load(const GffStruct &gffs) {
    loadTransform(gffs);
    loadBlueprint(gffs);
}

void Creature::loadTransform(const GffStruct &gffs) {
    _position[0] = gffs.getFloat("XPosition");
    _position[1] = gffs.getFloat("YPosition");
    _position[2] = gffs.getFloat("ZPosition");

    float dirX = gffs.getFloat("XOrientation");
    float dirY = gffs.getFloat("YOrientation");
    _facing = -glm::atan(dirX, dirY);

    updateTransform();
}

void Creature::loadBlueprint(const GffStruct &gffs) {
    string resRef(boost::to_lower_copy(gffs.getString("TemplateResRef")));
    shared_ptr<CreatureBlueprint> blueprint(Blueprints::instance().getCreature(resRef));
    load(blueprint);
}

void Creature::load(const shared_ptr<Blueprint<Creature>> &blueprint) {
    if (!blueprint) {
        throw invalid_argument("blueprint must not be null");
    }
    blueprint->load(*this);

    shared_ptr<TwoDaTable> appearance(Resources::instance().get2DA("appearance"));
    loadAppearance(*appearance, _appearance);
    loadPortrait(_appearance);
}

void Creature::loadAppearance(const TwoDaTable &table, int row) {
    _appearance = row;
    _modelType = parseModelType(table.getString(row, "modeltype"));
    _walkSpeed = table.getFloat(row, "walkdist", 0.0f);
    _runSpeed = table.getFloat(row, "rundist", 0.0f);

    updateModel();
}

Creature::ModelType Creature::parseModelType(const string &s) const {
    if (s == "S" || s == "L") {
        return ModelType::Creature;
    } else if (s == "F") {
        return ModelType::Droid;
    } else if (s == "B") {
        return ModelType::Character;
    }

    throw logic_error("Unsupported model type: " + s);
}

void Creature::updateModel() {
    if (_model) {
        _sceneGraph->removeRoot(_model);
    }
    _model = _modelBuilder.build();

    if (_model) {
        _headModel = _model->getAttachedModel(g_headHookNode);
        if (!_stunt) {
            _model->setLocalTransform(_transform);
        }
        _sceneGraph->addRoot(_model);
        _animDirty = true;
    }
}

void Creature::loadPortrait(int appearance) {
    shared_ptr<TwoDaTable> portraits(Resources::instance().get2DA("portraits"));
    string appearanceString(to_string(appearance));

    const TwoDaRow *row = portraits->findRow([&appearanceString](const TwoDaRow &r) {
        return
            r.getString("appearancenumber") == appearanceString ||
            r.getString("appearance_s") == appearanceString ||
            r.getString("appearance_l") == appearanceString;
    });
    if (!row) {
        warn("Creature: portrait not found: " + appearanceString);
        return;
    }
    string resRef(row->getString("baseresref"));
    boost::to_lower(resRef);

    _portrait = Textures::instance().get(resRef, TextureType::GUI);
}

void Creature::update(float dt) {
    SpatialObject::update(dt);
    updateModelAnimation();
    updateHealth();
}

void Creature::updateModelAnimation() {
    if (!_model) return;

    if (_animFireForget) {
        if (!_model->isAnimationFinished()) return;

        _animFireForget = false;
        _animDirty = true;
    }
    if (!_animDirty) return;

    if (_animAction) {
        _animAction->complete();
        _animAction.reset();
    }
    switch (_movementType) {
        case MovementType::Run: {
            shared_ptr<Animation> anim(_model->model()->getAnimation(_animResolver.getRunAnimation()));
            if (anim) {
                _model->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                if (_headModel) {
                    _headModel->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                }
            }
            break;
        }
        case MovementType::Walk: {
            shared_ptr<Animation> anim(_model->model()->getAnimation(_animResolver.getWalkAnimation()));
            if (anim) {
                _model->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                if (_headModel) {
                    _headModel->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                }
            }
            break;
        }
        default:
            if (_dead) {
                shared_ptr<Animation> anim(_model->model()->getAnimation(_animResolver.getDeadAnimation()));
                if (anim) {
                    _model->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                    if (_headModel) {
                        _headModel->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                    }
                }
            } else if (_talking) {
                shared_ptr<Animation> anim(_model->model()->getAnimation(_animResolver.getTalkNormalAnimation()));
                if (anim) {
                    _model->playAnimation(anim, AnimationFlags::loop, 1.0f, _model->model()->animationScale());
                    if (_headModel) {
                        _headModel->playAnimation(anim, AnimationFlags::loop, 1.0f, _model->model()->animationScale());
                        _headModel->playAnimation(_animResolver.getHeadTalkAnimation(), AnimationFlags::loopOverlay, 0.25f);
                    }
                }
            } else {
                shared_ptr<Animation> anim(_model->model()->getAnimation(_animResolver.getPauseAnimation()));
                if (anim) {
                    _model->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                    if (_headModel) {
                        _headModel->playAnimation(anim, AnimationFlags::loopBlend, 1.0f, _model->model()->animationScale());
                    }
                }
            }
            break;
    }

    _animDirty = false;
}

void Creature::updateHealth() {
    if (_currentHitPoints > 0 || _immortal || _dead) return;

    die();
}

void Creature::clearAllActions() {
    SpatialObject::clearAllActions();
    setMovementType(MovementType::None);
}

void Creature::playAnimation(AnimationType anim, float speed, shared_ptr<Action> actionToComplete) {
    string animName(_animResolver.getAnimationName(anim));
    if (animName.empty()) return;

    int flags = isAnimationLooping(anim) ? AnimationFlags::loop : 0;
    playAnimation(animName, flags, speed, actionToComplete);
}

void Creature::playAnimation(const string &name, int flags, float speed, shared_ptr<Action> actionToComplete) {
    doPlayAnimation(flags, [&]() {
        _animAction = actionToComplete;
        _model->playAnimation(name, flags, speed);
        if (_headModel && (flags & AnimationFlags::propagateHead)) {
            _headModel->playAnimation(name, flags & ~AnimationFlags::propagateHead, speed);
        }
    });
}

void Creature::doPlayAnimation(int flags, const function<void()> &callback) {
    if (!_model || _movementType != MovementType::None) return;

    callback();

    if (!(flags & AnimationFlags::loop)) {
        _animFireForget = true;
    }
}

void Creature::playAnimation(const shared_ptr<Animation> &anim, int flags, float speed) {
    doPlayAnimation(flags, [&]() {
        // TODO: scale should be computed from this creatures model and the animations model
        _model->playAnimation(anim, flags, speed, 1.0f);
        if (_headModel && (flags & AnimationFlags::propagateHead)) {
            _headModel->playAnimation(anim, flags & ~AnimationFlags::propagateHead, speed, 1.0f);
        }
    });
}

void Creature::playAnimation(CombatAnimation anim, CreatureWieldType wield, int variant) {
    string animName(_animResolver.getAnimationName(anim, wield, variant));
    if (!animName.empty()) {
        playAnimation(animName);
    }
}

bool Creature::equip(const string &resRef) {
    shared_ptr<ItemBlueprint> blueprint(Blueprints::instance().getItem(resRef));

    shared_ptr<Item> item(_objectFactory->newItem());
    item->load(blueprint);

    bool equipped = false;

    if (item->isEquippable(kInventorySlotBody)) {
        equipped = equip(kInventorySlotBody, item);
    } else if (item->isEquippable(kInventorySlotRightWeapon)) {
        equipped = equip(kInventorySlotRightWeapon, item);
    }

    return equipped;
}

bool Creature::equip(InventorySlot slot, const shared_ptr<Item> &item) {
    if (!item->isEquippable(slot)) return false;

    _equipment[slot] = item;
    item->setEquipped(true);

    if (_model) {
        updateModel();

        if (slot == InventorySlot::kInventorySlotRightWeapon) {
            shared_ptr<ModelSceneNode> weapon(_model->getAttachedModel("rhand"));
            if (weapon && weapon->model()->classification() == Model::Classification::Lightsaber) {
                weapon->setDefaultAnimation("powered");
                weapon->playAnimation("powerup");
            }
        }
    }

    return true;
}

void Creature::unequip(const shared_ptr<Item> &item) {
    for (auto &equipped : _equipment) {
        if (equipped.second == item) {
            item->setEquipped(false);
            _equipment.erase(equipped.first);

            if (_model) {
                updateModel();
            }
            break;
        }
    }
}

shared_ptr<Item> Creature::getEquippedItem(InventorySlot slot) const {
    auto equipped = _equipment.find(slot);
    return equipped != _equipment.end() ? equipped->second : nullptr;
}

bool Creature::isSlotEquipped(InventorySlot slot) const {
    return _equipment.find(slot) != _equipment.end();
}

void Creature::setMovementType(MovementType type) {
    if (_movementType == type) return;

    _movementType = type;
    _animDirty = true;
    _animFireForget = false;
}

void Creature::setTalking(bool talking) {
    if (_talking == talking) return;

    _talking = talking;
    _animDirty = true;
}

void Creature::setPath(const glm::vec3 &dest, vector<glm::vec3> &&points, uint32_t timeFound) {
    int pointIdx = 0;
    if (_path) {
        bool lastPointReached = _path->pointIdx == _path->points.size();
        if (lastPointReached) {
            float nearestDist = INFINITY;
            for (int i = 0; i < points.size(); ++i) {
                float dist = glm::distance2(_path->destination, points[i]);
                if (dist < nearestDist) {
                    nearestDist = dist;
                    pointIdx = i;
                }
            }
        } else {
            const glm::vec3 &nextPoint = _path->points[_path->pointIdx];
            for (int i = 0; i < points.size(); ++i) {
                if (points[i] == nextPoint) {
                    pointIdx = i;
                    break;
                }
            }
        }
    }
    auto path = make_unique<Path>();
    path->destination = dest;
    path->points = points;
    path->timeFound = timeFound;
    path->pointIdx = pointIdx;

    _path = move(path);
}

void Creature::clearPath() {
    _path.reset();
}

glm::vec3 Creature::getSelectablePosition() const {
    if (_dead) return _model->getCenterOfAABB();

    glm::vec3 position;

    if (_model->getNodeAbsolutePosition(g_talkDummyNode, position)) {
        return _model->absoluteTransform() * glm::vec4(position, 1.0f);
    }

    return _model->getCenterOfAABB();
}

float Creature::getAttackRange() const {
    float result = kDefaultAttackRange;

    shared_ptr<Item> item(getEquippedItem(kInventorySlotRightWeapon));
    if (item && item->attackRange() > kDefaultAttackRange) {
        result = item->attackRange();
    }

    return result;
}

void Creature::setFaction(Faction faction) {
    _faction = faction;
}

bool Creature::isLevelUpPending() const {
    int level = _attributes.getAggregateLevel();
    return _xp >= level * (level + 1) * 500;
}

void Creature::setMovementRestricted(bool restricted) {
    _movementRestricted = restricted;
}

void Creature::setInCombat(bool inCombat) {
    _inCombat = inCombat;
}

void Creature::setImmortal(bool immortal) {
    _immortal = immortal;
}

void Creature::setXP(int xp) {
    _xp = xp;
}

void Creature::runSpawnScript() {
    if (!_onSpawn.empty()) {
        _scriptRunner->run(_onSpawn, _id, kObjectInvalid);
    }
}

void Creature::giveXP(int amount) {
    _xp += amount;
}

void Creature::die() {
    _currentHitPoints = 0;
    _dead = true;
    _name = Resources::instance().getString(kStrRefRemains);

    debug(boost::format("Creature: '%s' is dead") % _tag, 2);

    playAnimation(_animResolver.getDieAnimation());
    runDeathScript();
}

void Creature::runDeathScript() {
    if (!_onDeath.empty()) {
        _scriptRunner->run(_onDeath, _id, kObjectInvalid);
    }
}

CreatureWieldType Creature::getWieldType() const {
    auto rightWeapon = getEquippedItem(InventorySlot::kInventorySlotRightWeapon);
    auto leftWeapon = getEquippedItem(InventorySlot::kInventorySlotLeftWeapon);

    if (rightWeapon && leftWeapon) {
        return (rightWeapon->weaponWield() == WeaponWield::BlasterPistol) ? CreatureWieldType::DualPistols : CreatureWieldType::DualSwords;
    } else if (rightWeapon) {
        switch (rightWeapon->weaponWield()) {
            case WeaponWield::SingleSword:
                return CreatureWieldType::SingleSword;
            case WeaponWield::DoubleBladedSword:
                return CreatureWieldType::DoubleBladedSword;
            case WeaponWield::BlasterPistol:
                return CreatureWieldType::BlasterPistol;
            case WeaponWield::BlasterRifle:
                return CreatureWieldType::BlasterRifle;
            case WeaponWield::HeavyWeapon:
                return CreatureWieldType::HeavyWeapon;
            case WeaponWield::StunBaton:
            default:
                return CreatureWieldType::StunBaton;
        }
    }

    return CreatureWieldType::HandToHand;
}

} // namespace game

} // namespace reone
