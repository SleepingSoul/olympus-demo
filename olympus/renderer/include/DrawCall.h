#pragma once

namespace rdr
{
    struct VoxelDrawCall
    {
        glm::vec4 color;
        glm::vec3 position;
        glm::vec3 rotationVec;
        float angle;
    };
}