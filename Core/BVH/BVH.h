
// Class and functionalities for a Bounding Volume Hierarchy data structure
// Reference: https://jacco.ompf2.com/

// This is a non-sparse (contiguous) BVH implementation, because is based on non-sparse BVH node data structure

#pragma once

#include <vector>

#include <Core/MeshData.h>

namespace GaladHen
{
	struct BVHNode;
	class Mesh;
	class Scene;
	struct Ray;
	struct RayTriangleMeshHitInfo;

	class BVH
	{
	public:

		BVH();

		// @brief
		// Build the BVH for a mesh, changing order of indices inside it (in-place)
		void BuildBVH(Mesh& mesh);

		// @brief
		// Build the BVH from a scene
		void BuildBVH(const Scene& scene);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry
		// @param ray: the ray casted
		// @param mesh: the mesh used to perform intersection tests on actual geometry -> this MUST be the same mesh used when the bvh was builded
		// @returns infos about intersection
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry, strarting from a specific node
		// @param ray: the ray casted
		// @param mesh: the mesh used to perform intersection tests on actual geometry -> this MUST be the same mesh used when the bvh was builded
		// @param node: starting node of the intersection tests
		// @returns infos about intersection
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVHNode& node);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry, strarting from a specific node index
		// @param ray: the ray casted
		// @param mesh: the mesh used to perform intersection tests on actual geometry -> this MUST be the same mesh used when the bvh was builded
		// @param nodeIndex: starting node index of the intersection tests
		// @returns infos about intersection
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const unsigned int nodeIndex);

		BVHNode& GetRootNode();
		
		BVHNode& GetNode(unsigned int index);

		unsigned int GetNodeNumber() const;

	protected:

		void LongestAxisSubdivision(BVHNode& node, Mesh& mesh);

		std::vector<BVHNode> Nodes;
		unsigned int RootNode;

	};
}