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

#pragma once

#include "path.h"

namespace reone {

namespace game {

class Pathfinder : boost::noncopyable {
public:
    void load(const std::vector<Path::Point> &points, const std::unordered_map<int, float> &pointZ);

    const std::vector<glm::vec3> findPath(const glm::vec3 &from, const glm::vec3 &to) const;

    const std::vector<glm::vec3> &vertices() const { return _vertices; }

private:
    struct Edge {
        uint16_t toIndex { 0 };
        float length { 0 };

        Edge(uint16_t toIndex, float length);
    };

    struct FindPathContext {
        std::map<uint16_t, std::pair<uint16_t, float>> fromToDistance;
        std::set<uint16_t> visited;
        std::queue<uint16_t> queue;
        // std::priority_queue<uint16_t, std::vector<uint16_t>, std::greater<uint16_t>> queue;
    };

    std::vector<glm::vec3> _vertices;
    std::unordered_map<uint16_t, std::vector<Edge>> _edges;

    uint16_t getNearestVertex(const glm::vec3 &point) const;
    void visit(uint16_t index, FindPathContext &ctx) const;
};

} // namespace game

} // namespace reone
