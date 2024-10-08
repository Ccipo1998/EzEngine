
// Mesh class representing the geometry of an object with its vertex data and mesh ID

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "MeshData.h"

#include "BVH/BVH.h"

namespace GaladHen
{
    class Mesh
    {
    public:

        // @brief
        // Default constructor: the MeshID is defaulted at zero
        Mesh();

        // Copy constructor
        Mesh(const Mesh& other) noexcept;
        // Copy assignment
        Mesh& operator=(const Mesh& other) noexcept;
        // Move constructor
        Mesh(Mesh&& other) noexcept;
        // Move assignment
        Mesh& operator=(Mesh&& other) noexcept;

        std::vector<VertexData> Vertices;
        std::vector<unsigned int> Indices;
        Primitive PrimitiveType;

        BVH MeshBVH;

        unsigned int MeshID; // 0 means -1, so the low level pair is not already been created
    };
}
