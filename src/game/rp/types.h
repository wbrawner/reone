/*
 * Copyright (c) 2020 The reone project contributors
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

#pragma once

namespace reone {

namespace game {

enum class Gender {
    Male = 0,
    Female = 1,
    Both = 2,
    Other = 3,
    None = 4
};

enum class ClassType {
    Soldier = 0,
    Scout = 1,
    Scoundrel = 2,
    JediGuardian = 3,
    JediConsular = 4,
    JediSentinel = 5,
    CombatDroid = 6,
    ExpertDroid = 7,
    Minion = 8,
    TechSpecialist = 9,
    BountyHunter = 10,
    JediWeaponMaster = 11,
    JediMaster = 12,
    JediWatchman = 13,
    SithMarauder = 14,
    SithLord = 15,
    SithAssassin = 16,

    Invalid = 255
};

enum class Faction {
    Invalid = -1,
    Hostile1 = 1,
    Friendly1 = 2,
    Hostile2 = 3,
    Friendly2 = 4,
    Neutral = 5,
    Insane = 6,
    Tuskan = 7,
    GlobalXor = 8,
    Surrender1 = 9,
    Surrender2 = 10,
    Predator = 11,
    Prey = 12,
    Trap = 13,
    EndarSpire = 14,
    Rancor = 15,
    Gizka1 = 16,
    Gizka2 = 17,
    SelfLoathing = 21,
    OneOnOne = 22,
    PartyPuppet = 23
};

enum class Ability {
    Strength = 0,
    Dexterity = 1,
    Constitution = 2,
    Intelligence = 3,
    Wisdom = 4,
    Charisma = 5
};

enum class Skill {
    ComputerUse = 0,
    Demolitions = 1,
    Stealth = 2,
    Awareness = 3,
    Persuade = 4,
    Repair = 5,
    Security = 6,
    TreatInjury = 7
};

enum class Feat {
    AdvancedJediDefense = 1,
    AdvancedGuardStance = 2,
    Ambidexterity = 3,
    ArmourProfHeavy = 4,
    ArmourProfLight = 5,
    ArmourProfMedium = 6,
    Cautious = 7,
    CriticalStrike = 8,
    DoubleWeaponFighting = 9,
    Empathy = 10,
    Flurry = 11,
    GearHead = 12,
    GreatFortitude = 13,
    ImplantLevel1 = 14,
    ImplantLevel2 = 15,
    ImplantLevel3 = 16,
    ImprovedPowerAttack = 17,
    ImprovedPowerBlast = 18,
    ImprovedCriticalStrike = 19,
    ImprovedSniperShot = 20,
    IronWill = 21,
    LightningReflexes = 22,
    MasterJediDefense = 24,
    MasterGuardStance = 25,
    MultiShot = 26,
    Perceptive = 27,
    PowerAttack = 28,
    PowerBlast = 29,
    RapidShot = 30,
    SniperShot = 31,
    WeaponFocusBlaster = 32,
    WeaponFocusBlasterRifle = 33,
    WeaponFocusGrenade = 34,
    WeaponFocusHeavyWeapons = 35,
    WeaponFocusLightsaber = 36,
    WeaponFocusMeleeWeapons = 37,
    WeaponFocusSimpleWeapons = 38,
    WeaponProficiencyBlaster = 39,
    WeaponProficiencyBlasterRifle = 40,
    WeaponProficiencyGrenade = 41,
    WeaponProficiencyHeavyWeapons = 42,
    WeaponProficiencyLightsaber = 43,
    WeaponProficiencyMeleeWeapons = 44,
    WeaponProficiencySimpleWeapons = 45,
    WeaponSpecializationBlaster = 46,
    WeaponSpecializationBlasterRifle = 47,
    WeaponSpecializationGrenade = 48,
    WeaponSpecializationHeavyWeapons = 49,
    WeaponSpecializationLightsaber = 50,
    WeaponSpecializationMeleeWeapons = 51,
    WeaponSpecializationSimpleWeapons = 52,
    WhirlwindAttack = 53,
    GuardStance = 54,
    JediDefense = 55,
    UncannyDodge1 = 56,
    UncannyDodge2 = 57,
    SkillFocusComputerUse = 58,
    SneakAttack1d6 = 60,
    SneakAttack2d6 = 61,
    SneakAttack3d6 = 62,
    SneakAttack4d6 = 63,
    SneakAttack5d6 = 64,
    SneakAttack6d6 = 65,
    SneakAttack7d6 = 66,
    SneakAttack8d6 = 67,
    SneakAttack9d6 = 68,
    SneakAttack10d6 = 69,
    SkillFocusDemolitions = 70,
    SkillFocusStealth = 71,
    SkillFocusAwareness = 72,
    SkillFocusPersuade = 73,
    SkillFocusRepair = 74,
    SkillFocusSecurity = 75,
    SkillFocusTreatInjuury = 76,
    MasterSniperShot = 77,
    DroidUpgrade1 = 78,
    DroidUpgrade2 = 79,
    DroidUpgrade3 = 80,
    MasterCriticalStrike = 81,
    MasterPowerBlast = 82,
    MasterPowerAttack = 83,
    Toughness = 84,
    AdvancedDoubleWeaponFighting = 85,
    ForceFocusAlter = 86,
    ForceFocusControl = 87,
    ForceFocusSense = 88,
    ForceFocusAdvanced = 89,
    ForceFocusMastery = 90,
    ImprovedFlurry = 91,
    ImprovedRapidShot = 92,
    ProficiencyAll = 93,
    BattleMeditation = 94,

    // TSL

    Evasion = 125,
    Targeting1 = 126,
    Targeting2 = 127,
    Targeting3 = 128,
    Targeting4 = 129,
    Targeting5 = 130,
    Targeting6 = 131,
    Targeting7 = 132,
    Targeting8 = 133,
    Targeting9 = 134,
    Targeting10 = 135,
    CloseCombat = 139,
    ImprovedCloseCombat = 140,
    ImprovedForceCcamouflage = 141,
    MasterForceCamouflage = 142,
    RegenerateForcePoints = 143,
    DarkSideCorruption = 149,
    IgnorePain1 = 150,
    IgnorePain2 = 151,
    IgnorePain3 = 152,
    IncreaseCombatDamage1 = 153,
    IncreaseCombatDamage2 = 154,
    IncreaseCombatDamage3 = 155,
    SuperiorWeaponFocusLightsaber1 = 156,
    SuperiorWeaponFocusLightsaber2 = 157,
    SuperiorWeaponFocusLightsaber3 = 158,
    SuperiorWeaponFocusTwoWeapon1 = 159,
    SuperiorWeaponFocusTwoWeapon2 = 160,
    SuperiorWeaponFocusTwoWeapon3 = 161,
    LightSideEnlightenment = 167,
    Deflect = 168,
    InnerStrength1 = 169,
    InnerStrength2 = 170,
    InnerStrength3 = 171,
    IncreaseMeleeDamage1 = 172,
    IncreaseMeleeDamage2 = 173,
    IncreaseMeleeDamage3 = 174,
    Craft = 175,
    MastercraftWeapons1 = 176,
    MastercraftWeapons2 = 177,
    MastercraftWeapons3 = 178,
    MastercraftArmor1 = 179,
    MastercraftArmor2 = 180,
    MastercraftArmor3 = 181,
    DroidInterface = 182,
    ClassSkillAwareness = 183,
    ClassSkillComputerUse = 184,
    ClassSkillDemolitions = 185,
    ClassSkillRepair = 186,
    ClassSkillSecurity = 187,
    ClassSkillStealth = 188,
    ClassSkillTreatInjury = 189,
    DualStrike = 190,
    ImprovedDualStrike = 191,
    MasterDualStrike = 192,
    FinesseLightsabers = 193,
    FinesseMeleeWeapons = 194,
    Mobility = 195,
    RegenerateVitalityPoints = 196,
    StealthRun = 197,
    KineticCombat = 198,
    Survival = 199,
    MandalorianCourage = 200,
    PersonalCloakingShield = 201,
    Mentor = 202,
    ImplantSwitching = 203,
    Spirit = 204,
    ForceChain = 205,
    WarVeteran = 206,
    FightingSpirit = 236,
    HeroicResolve = 237,
    PreciseShot = 240,
    ImprovedPreciseShot = 241,
    MasterPreciseShot = 242,
    PreciseShotIV = 243,
    PreciseShotV = 244

    // END TSL
};

enum class ForcePower {
    All = -1,
    MasterAlter = 0,
    MasterControl = 1,
    MasterSense = 2,
    ForceJumpAdvanced = 3,
    LightSaberThrowAdvanced = 4,
    RegnerationAdvanced = 5,
    AffectMind = 6,
    Affliction = 7,
    SpeedBurst = 8,
    Choke = 9,
    Cure = 10,
    DeathField = 11,
    DroidDisable = 12,
    DroidDestroy = 13,
    Dominate = 14,
    DrainLife = 15,
    Fear = 16,
    ForceArmor = 17,
    ForceAura = 18,
    ForceBreach = 19,
    ForceImmunity = 20,
    ForceJump = 21,
    ForceMind = 22,
    ForcePush = 23,
    ForceShield = 24,
    ForceStorm = 25,
    ForceWave = 26,
    ForceWhirlwind = 27,
    Heal = 28,
    Hold = 29,
    Horror = 30,
    Insanity = 31,
    Kill = 32,
    KnightMind = 33,
    KnightSpeed = 34,
    Lightning = 35,
    MindMastery = 36,
    SpeedMastery = 37,
    Plague = 38,
    Regeneration = 39,
    ResistColdHeatEnergy = 40,
    ResistForce = 41,
    ResistPoisonDiseaseSonic = 42,
    Shock = 43,
    Sleep = 44,
    Slow = 45,
    Stun = 46,
    DroidStun = 47,
    SupressForce = 48,
    LightSaberThrow = 49,
    Wound = 50,
    BattleMeditation = 51,
    BodyFuel = 52,
    CombatRegeneration = 53,
    WarriorStance = 54,
    SentinelStance = 55,
    DominateMind = 56,
    PsychicStance = 57,
    CatharReflexes = 58,
    EnhancedSenses = 59,
    Camoflage = 60,
    Taunt = 61,
    WhirlingDervish = 62,
    Rage = 63,

    // TSL

    MasterEnergyResistance = 133,
    MasterHeal = 134,
    ForceBarrier = 135,
    ImprovedForceBarrier = 136,
    MasterForceBarrier = 137,
    BattleMeditationPC = 138,
    ImprovedBattleMeditationPC = 139,
    MasterBattleMeditationPC = 140,
    BatMedEnemy = 141,
    ImpBatMedEnemy = 142,
    MasBatMedEnemy = 143,
    CrushOppositionI = 144,
    CrushOppositionII = 145,
    CrushOppositionIII = 146,
    CrushOppositionIV = 147,
    CrushOppositionV = 148,
    CrushOppositionVI = 149,
    ForceBody = 150,
    ImprovedForceBody = 151,
    MasterForceBody = 152,
    DrainForce = 153,
    ImprovedDrainForce = 154,
    MasterDrainForce = 155,
    ForceCamouflage = 156,
    ImprovedForceCamouflage = 157,
    MasterForceCamouflage = 158,
    ForceScream = 159,
    ImprovedForceScream = 160,
    MasterForceScream = 161,
    ForceRepulsion = 162,
    ForceRedirection = 163,
    Fury = 164,
    ImprovedFury = 165,
    MasterFury = 166,
    InspireFollowersI = 167,
    InspireFollowersII = 168,
    InspireFollowersIII = 169,
    InspireFollowersIV = 170,
    InspireFollowersV = 171,
    InspireFollowersVI = 172,
    Revitalize = 173,
    ImprovedRevitalize = 174,
    MasterRevitalize = 175,
    ForceSight = 176,
    ForceCrush = 177,
    Precognition = 178,
    BattlePrecognition = 179,
    ForceEnlightenment = 180,
    MindTrick = 181,
    Confusion = 200,
    BeastTrick = 182,
    BeastConfusion = 184,
    DroidTrick = 201,
    DroidConfusion = 269,
    BreathControl = 270,
    WookieeRageI = 271,
    WookieeRageII = 272,
    WookieeRageIII = 273

    // END TSL
};

} // namespace game

} // namespace reone
