#pragma once
#include "../defines.h"
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <iostream>
#include "../utils/PerlinNoise.h"

namespace DZB
{

	struct ChunkRendererVertexData
	{
		glm::tvec3<uint8_t> position;
		glm::tvec2<uint8_t> uv;
		uint8_t light;
	};

	class Chunk
	{
	private:
		friend class World;
		static PerlinNoise perlinNoise;

	private:
		int id;
		bType blockType[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		bState blockState[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		bLight blockLight[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		cPos columnHeight[CHUNK_SIZE * CHUNK_SIZE];
		

	public:
		Chunk();
		Chunk(Chunk const&);
		Chunk(int x, int z);
		Chunk(int x, int z, bType blockID);
		~Chunk();

	public:
		unsigned int facesBuffered;

	private:
		static GLuint m_IBO;
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_IndexCount;
		ChunkRendererVertexData* m_Buffer;

	public:
		static void CreateIndexBuffer();
		static void DeleteIndexBuffer();

	public:
		void Init();

		void AddToBuffer(bTex textureID, cPos x, cPos y, cPos z, int u, int v, bLight light);

		void Begin();
		void End();

		void Bind();
		void DrawAll();
		void DrawRange(GLuint start, GLuint end);

		void DrawFace(bTex textureID, cPos x, cPos y, cPos z, Face face);

	public:
		inline const int GetX() { return id % CHUNK_MAX - CHUNK_MAX / 2; }
		inline const int GetZ() { return id / CHUNK_MAX - CHUNK_MAX / 2; }
		inline const int GetID() { return id; }

	private:
		bType BlockType(cPos x, cPos y, cPos z);
		bState BlockState(cPos x, cPos y, cPos z);
		bLight BlockLight(cPos x, cPos y, cPos z);
		bool Transparent(cPos x, cPos y, cPos z);

		//Get the bits XXXX0000
		inline int GetSunlight(int x, int y, int z) {

			return blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] >> 4 & 0xF;
		}

		// Set the bits XXXX0000
		inline void SetSunlight(int x, int y, int z, int val) {

			blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = (blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] & 0xF) | (val << 4);
		}

		// Get the bits 0000XXXX
		inline int GetTorchlight(int x, int y, int z) {

			return blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] & 0xF;
		}

		// Set the bits 0000XXXX
		inline void SetTorchlight(int x, int y, int z, int val) {

			blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = (blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] & 0xF0) | val;
		}

	public:
		static bTex Texture(bType blockType, bState blockState, Face face);
		static bool Transparent(bType blockType);
		static bool BreaksOnPlacement(bType heldBlockType, bType targetBlockType);

		uint8_t BreakBlock(int index);
		uint8_t PlaceBlock(int index, bType block);
	};
}