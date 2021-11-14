#pragma once
#include <unordered_map>
#include <queue>
#include "../defines.h"
#include "chunk.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"


namespace DZB
{
	struct RaycastHit
	{
	public:
		Chunk* chunk;
		cIndex index;
		Face face;

		RaycastHit() :
			chunk(nullptr),
			index(0),
			face(EAST)
		{ }

		RaycastHit(Chunk* chunk, cIndex index, Face face) :
			chunk(chunk),
			index(index),
			face(face)
		{ }

		inline glm::ivec3 BreakPosition()
		{
			int worldX = chunk->GetX() * CHUNK_SIZE + index % CHUNK_SIZE;
			int worldZ = chunk->GetZ() * CHUNK_SIZE + (index / CHUNK_SIZE) % CHUNK_SIZE;
			int worldY = (index / (CHUNK_SIZE * CHUNK_SIZE));

			return glm::ivec3(worldX, worldY, worldZ);
		}

		inline glm::ivec3 PlacePosition()
		{
			int worldX = chunk->GetX() * CHUNK_SIZE + index % CHUNK_SIZE;
			int worldZ = chunk->GetZ() * CHUNK_SIZE + (index / CHUNK_SIZE) % CHUNK_SIZE;
			int worldY = (index / (CHUNK_SIZE * CHUNK_SIZE));

			if (face == EAST)
				return glm::ivec3(worldX - 1, worldY, worldZ);
			if (face == WEST)
				return glm::ivec3(worldX + 1, worldY, worldZ);
			if (face == TOP)
				return glm::ivec3(worldX, worldY - 1, worldZ);
			if (face == TOP)
				return glm::ivec3(worldX, worldY + 1, worldZ);
			if (face == NORTH)
				return glm::ivec3(worldX, worldY, worldZ - 1);
			if (face == SOUTH)
				return glm::ivec3(worldX, worldY, worldZ + 1);

			return glm::ivec3();
		}
	};

	

	class World
	{
	public:
		static World& GetInstance()
		{
			static World instance;
			return instance;
		}
	private:
		World() {}
	public:
		World(World const&) = delete;
		void operator=(World const&) = delete;
	private:
		std::unordered_map<int, Chunk*> chunks;
	public:
		void AddChunk(int chunkX, int chunkZ);
		void AddChunk(int chunkX, int chunkZ, bType blockID);
		void DeleteChunk(int chunkX, int chunkZ);
		void DeleteChunk(Chunk* chunk);
		Chunk* GetChunk(int id);
		Chunk* GetChunk(int chunkX, int chunkZ);
		bType BlockType(int x, int y, int z);
		bool RaycastBlock(const glm::vec3& origin, const glm::vec3& direction, float maxLength, int maxSteps, RaycastHit& out);

		void BufferChunks();
		void BufferChunk(Chunk* chunk);

		void RenderChunks(Shader* shader, Texture* texture, glm::vec4* frustum);

		void BlockUpdate(Chunk* chunk, cPos x, cPos y, cPos z);

		bType BreakBlock(RaycastHit hit);
		bType BreakBlock(int worldX, int worldY, int worldZ);

		int PlaceBlock(bType heldBlock, RaycastHit hit);
		int PlaceBlock(bType heldBlock, int worldX, int worldY, int worldZ);
		bLight GetLight(int worldX, int worldY, int worldZ);
		bLight GetVertexLight(int vX, int vY, int vZ, int worldX, int worldY, int worldZ, Face face);
		bLight GetAverageLight(glm::ivec3 a, glm::ivec3 b, glm::ivec3 c, glm::ivec3 d);

		void DrawChunkBoundaries();

		unsigned int GetFacesBuffered()
		{
			unsigned int faces = 0;
			for (auto& it : chunks)
				faces += it.second->facesBuffered;
			return faces;
		}

		unsigned int ChunksCount() { return chunks.size(); }
	private:
		void CalculateLightDown(Chunk* chunk);
		void CalculateLightQueue(Chunk* chunk, std::queue<glm::ivec3>& queue);
		void CalculateLightFlood(std::queue<glm::ivec3>& queue);

	public:
		static float intbound(float s, float ds)
		{
			// Find the smallest positive t such that s+t*ds is an integer.
			if (ds < 0)
			{
				return intbound(-s, -ds);
			}
			else
			{
				s = mod(s, 1);
				// problem is now s+t*ds = 1
				return (1.0f - s) / ds;
			}
		}

		static int signum(float x)
		{
			return x == 0 ? 0 : x < 0 ? -1 : 1;
		}

		static float mod(float value, int modulus)
		{
			return fmod(fmod(value, modulus) + modulus, modulus);
		}
	};


	

	struct BlockRef
	{
	public:
		Chunk* chunk;
		const cIndex index;

		BlockRef(Chunk* chunk, cIndex index) :
			chunk(chunk),
			index(index)
		{ }

		BlockRef(int chunkID, cIndex index) :
			chunk(World::GetInstance().GetChunk(chunkID)),
			index(index)
		{}

		BlockRef(int worldX, int worldY, int worldZ) :
			chunk(World::GetInstance().GetChunk(worldX >> 4, worldZ >> 4)),
			index((worldX - (worldX >> 4) * CHUNK_SIZE) + (worldZ - (worldZ >> 4) * CHUNK_SIZE) * CHUNK_SIZE + worldY * CHUNK_SIZE * CHUNK_SIZE)
		{ }

		inline glm::ivec3 GetWorldPos()
		{
			int worldX = chunk->GetX() * CHUNK_SIZE + index % CHUNK_SIZE;
			int worldZ = chunk->GetZ() * CHUNK_SIZE + (index / CHUNK_SIZE) % CHUNK_SIZE;
			int worldY = (index / (CHUNK_SIZE * CHUNK_SIZE));

			return glm::ivec3(worldX, worldY, worldZ);
		}
	};

	struct ChunkPos
	{
	public:
		const int chunkID;
		const cIndex index;
		const Face face;

		ChunkPos(int chunkID, cIndex index, Face face) :
			chunkID(chunkID),
			index(index),
			face(face)
		{ }

		ChunkPos(int worldX, int worldY, int worldZ, Face face) :
			chunkID(((worldX >> 4) + CHUNK_MAX / 2) + ((worldZ >> 4) + CHUNK_MAX / 2) * CHUNK_MAX),
			index((worldX - (worldX >> 4) * CHUNK_SIZE) + (worldZ - (worldZ >> 4) * CHUNK_SIZE) * CHUNK_SIZE + worldY * CHUNK_SIZE * CHUNK_SIZE),
			face(face)
		{ }

		inline glm::ivec3 GetWorldPos()
		{
			int worldX = (chunkID % CHUNK_MAX - CHUNK_MAX / 2) * CHUNK_SIZE + index % CHUNK_SIZE;
			int worldZ = (chunkID / CHUNK_MAX - CHUNK_MAX / 2) * CHUNK_SIZE + (index / CHUNK_SIZE) % CHUNK_SIZE;
			int worldY = (index / (CHUNK_SIZE * CHUNK_SIZE));

			return glm::ivec3(worldX, worldY, worldZ);
		}

		Chunk* GetChunk()
		{
			return World::GetInstance().GetChunk(chunkID);
		}
	};
}