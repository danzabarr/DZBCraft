#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "../defines.h"

namespace DZB
{
	class FrustumCulling
	{
	public:
		static void GetPlanes(glm::mat4& viewProj, glm::vec4* planes)
		{
			// x, y, z, and w represent A, B, C and D in the plane equation
			// where ABC are the xyz of the planes normal, and D is the plane constant

			glm::float32_t* elements = glm::value_ptr(viewProj);

			// Left Frustum Plane
			// Add first column of the matrix to the fourth column
			planes[0].x = elements[3] + elements[0];
			planes[0].y = elements[7] + elements[4];
			planes[0].z = elements[11] + elements[8];
			planes[0].w = elements[15] + elements[12];
									   
			// Right Frustum Plane			   
			// Subtract first column of matrix from the fourth column
			planes[1].x = elements[3] - elements[0];
			planes[1].y = elements[7] - elements[4];
			planes[1].z = elements[11] - elements[8];
			planes[1].w = elements[15] - elements[12];
									   
			// Top Frustum Plane			   
			// Subtract second column of matrix from the fourth column
			planes[2].x = elements[3] - elements[1];
			planes[2].y = elements[7] - elements[5];
			planes[2].z = elements[11] - elements[9];
			planes[2].w = elements[15] - elements[13];
									   
			// Bottom Frustum Plane			   
			// Add second column of the matrix to the fourth column
			planes[3].x = elements[3] + elements[1];
			planes[3].y = elements[7] + elements[5];
			planes[3].z = elements[11] + elements[9];
			planes[3].w = elements[15] + elements[13];
									   
			// Near Frustum Plane			   
			// We could add the third column to the fourth column to get the near plane,
			// but we don't have to do this because the third column IS the near plane
			planes[4].x = elements[3] + elements[2];
			planes[4].y = elements[7] + elements[6];
			planes[4].z = elements[11] + elements[10];
			planes[4].w = elements[15] + elements[14];

			// Far Frustum Plane
			// Subtract third column of matrix from the fourth column
			planes[5].x = elements[3] - elements[2];
			planes[5].y = elements[7] - elements[6];
			planes[5].z = elements[11] - elements[10];
			planes[5].w = elements[15] - elements[14];

			// Normalize plane normals (A, B and C (xyz))
			// Also take note that planes face inward
			for (int i = 0; i < 6; ++i)
			{
				float length = sqrt((planes[i].x * planes[i].x) + (planes[i].y * planes[i].y) + (planes[i].z * planes[i].z));
				planes[i].x /= length;
				planes[i].y /= length;
				planes[i].z /= length;
				planes[i].w /= length;
			}
		}

		static bool CullBlock(glm::vec4* frustumPlanes, int x, int y, int z)
		{
			int minX = x;
			int maxX = x + 1;
			int minY = y;
			int maxY = y + 1;
			int minZ = z;
			int maxZ = z + 1;

			// Loop through each frustum plane
			for (int planeID = 0; planeID < 6; planeID++)
			{
				glm::vec4 plane = frustumPlanes[planeID];


				if (plane.x * minX + plane.y * minY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * minY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * maxY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * maxY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * minY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * minY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * maxY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * maxY + plane.z * maxZ + plane.w > 0)
					continue;
			

				/*



				glm::vec3 planeNormal = glm::vec3(frustumPlanes[planeID].x, frustumPlanes[planeID].y, frustumPlanes[planeID].z);
				float planeConstant = frustumPlanes[planeID].w;

				// Check each axis (x,y,z) to get the AABB vertex furthest away from the direction the plane is facing (plane normal)
				glm::vec3 axisVert;

				// x-axis
				if (frustumPlanes[planeID].x < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the x axis
					axisVert.x = x; // min x plus tree positions x
				else
					axisVert.x = x + 1; // max x plus tree positions x

				// y-axis
				if (frustumPlanes[planeID].y < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the y axis
					axisVert.y = y; // min y plus tree positions y
				else
					axisVert.y = y + 1; // max y plus tree positions y

				// z-axis
				if (frustumPlanes[planeID].z < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the z axis
					axisVert.z = z; // min z plus tree positions z
				else
					axisVert.z = z + 1; // max z plus tree positions z

				// Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
				// and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
				// that it should be culled
				if (glm::dot(planeNormal, axisVert) + planeConstant < 0.0f)
				{
				
					return true;
				}
				*/
				return true;
			}
			return false;
		}

		static bool CullChunk(glm::vec4* frustumPlanes, int x, int z)
		{
			int minX = x * CHUNK_SIZE;
			int maxX = x * CHUNK_SIZE + CHUNK_SIZE;
			int minY = 0;
			int maxY = CHUNK_HEIGHT;
			int minZ = z * CHUNK_SIZE;
			int maxZ = z * CHUNK_SIZE + CHUNK_SIZE;

			// Loop through each frustum plane
			for (int planeID = 0; planeID < 6; planeID++)
			{
				glm::vec4 plane = frustumPlanes[planeID];

				if (plane.x * minX + plane.y * minY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * minY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * maxY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * maxY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * minY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * minY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * maxY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * maxY + plane.z * maxZ + plane.w > 0)
					continue;

				return true;
			}
			return false;
		}

		static bool CullSection(glm::vec4* frustumPlanes, int x, int y, int z)
		{
			int minX = x * CHUNK_SIZE;
			int maxX = x * CHUNK_SIZE + CHUNK_SIZE;
			int minY = y * CHUNK_SIZE;
			int maxY = y * CHUNK_SIZE + CHUNK_SIZE;
			int minZ = z * CHUNK_SIZE;
			int maxZ = z * CHUNK_SIZE + CHUNK_SIZE;

			// Loop through each frustum plane
			for (int planeID = 0; planeID < 6; planeID++)
			{
				glm::vec4 plane = frustumPlanes[planeID];


				if (plane.x * minX + plane.y * minY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * minY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * maxY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * maxY + plane.z * minZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * minY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * minY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * minX + plane.y * maxY + plane.z * maxZ + plane.w > 0)
					continue;

				if (plane.x * maxX + plane.y * maxY + plane.z * maxZ + plane.w > 0)
					continue;

				return true;
			}
			return false;
		}

	};
}