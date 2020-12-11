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

#include "spatial.h"

#include "../../resource/gfffile.h"

#include "../camera/camera.h"

namespace reone {

namespace game {

class CameraObject : public SpatialObject, public Camera {
public:
    CameraObject(uint32_t id, ObjectFactory *objectFactory, scene::SceneGraph *sceneGraph);

    void load(const resource::GffStruct &gffs);

    int cameraId() const;
    float fieldOfView() const;

private:
    int _cameraId { 0 };
    float _fieldOfView { 0.0f };
};

} // namespace game

} // namespace reone
