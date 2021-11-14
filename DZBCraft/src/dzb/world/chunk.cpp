#include "chunk.h"
#include "world.h"

namespace DZB
{
	PerlinNoise Chunk::perlinNoise(0);

	Chunk::Chunk()
	{
		LOG("CH_CTR [" << GetX() << ", " << GetZ() << "] 0");
	}

	
	Chunk::Chunk(Chunk const&)
	{
		LOG("CH_CTR [" << GetX() << ", " << GetZ() << "] 1");
	}
	
	
	Chunk::Chunk(int x, int z) :
		id((x + CHUNK_MAX / 2) + (z + CHUNK_MAX / 2) * CHUNK_MAX)
	{
		LOG("CH_CTR [" << x << ", " << z << "] 2");
		Init();

		int cX = GetX() * CHUNK_SIZE;
		int cZ = GetZ() * CHUNK_SIZE;

		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				int height = perlinNoise.octaveNoise(((double)x + (double)cX) / 256.0, ((double)z + (double)cZ) / 256.0, 5) * 64.0 + 128.0;
				int levels = 0;

				for (int y = CHUNK_HEIGHT - 1 - height; y < CHUNK_HEIGHT - 1; y++) {
					if (levels == 0) blockType[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = GRASS;
					else if (levels < 3) blockType[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = DIRT;
					else blockType[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = STONE;
					levels++;
				}
			}
		}

		for (int z = 0; z < CHUNK_SIZE; z++)
			for (int x = 0; x < CHUNK_SIZE; x++)
				blockType[x + z * CHUNK_SIZE + (CHUNK_HEIGHT - 1) * CHUNK_SIZE * CHUNK_SIZE] = BEDROCK;

		for (int z = 0; z < CHUNK_SIZE; z++)
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				for (int x = 0; x < CHUNK_SIZE; x++)
					blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = x;
				
	}

	Chunk::Chunk(int x, int z, bType blockID) :
		id((x + CHUNK_MAX / 2) + (z + CHUNK_MAX / 2) * CHUNK_MAX)
	{
		LOG("CH_CTR [" << x << ", " << z << "] 3");
		Init();
		for (int z = 0; z < CHUNK_SIZE; z++)
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				for (int x = 0; x < CHUNK_SIZE; x++)
					blockType[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE] = blockID;//(unsigned char)((x + y + z) % 2) ? blockID : 0;
					
		for (int z = 0; z < CHUNK_SIZE; z++)
			for (int x = 0; x < CHUNK_SIZE; x++)
				blockType[x + z * CHUNK_SIZE + (CHUNK_HEIGHT - 1) * CHUNK_SIZE * CHUNK_SIZE] = 1;
	}

	Chunk::~Chunk()
	{
		LOG("CH_DTR [" << GetX() << ", " << GetZ() << "]");
	}

	GLuint Chunk::m_IBO = 0;

	void Chunk::CreateIndexBuffer()
	{
		GLuint* indices = new GLuint[CHUNK_RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < CHUNK_RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, CHUNK_RENDERER_INDICES_SIZE * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indices;
	}

	void Chunk::DeleteIndexBuffer()
	{
		glDeleteBuffers(1, &m_IBO);
	}

	void Chunk::Init()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, CHUNK_RENDERER_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, CHUNK_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(ChunkRendererVertexData, ChunkRendererVertexData::position)));
		glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, CHUNK_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(ChunkRendererVertexData, ChunkRendererVertexData::uv)));
		glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, CHUNK_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(ChunkRendererVertexData, ChunkRendererVertexData::light)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void Chunk::Begin()
	{
		facesBuffered = 0;

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (ChunkRendererVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		if (m_Buffer == nullptr)
			std::cout << "WARNING! glMapBuffer returned NULL" << std::endl;

		m_IndexCount = 0;
	}

	void Chunk::AddToBuffer(bTex textureID, cPos x, cPos y, cPos z, int u, int v, bLight light)
	{
		if (m_Buffer == nullptr) return;

		m_Buffer->position = glm::tvec3<uint8_t>(x, y, z);
		m_Buffer->uv = glm::tvec2<uint8_t>(u + textureID % 16, 16 - v - textureID / 16);
		m_Buffer->light = light;
		m_Buffer++;
	}

	void Chunk::DrawFace(bTex textureID, cPos x, cPos y, cPos z, Face face)
	{
		facesBuffered++;

		int worldX = x + GetX() * CHUNK_SIZE;
		int worldY = y;
		int worldZ = z + GetZ() * CHUNK_SIZE;

		if (face == EAST)
		{
			AddToBuffer(textureID, x + 0, y + 0, z + 1, 0, 0, World::GetInstance().GetVertexLight(0, 0, 1, worldX, worldY, worldZ, EAST));
			AddToBuffer(textureID, x + 0, y + 1, z + 1, 0, 1, World::GetInstance().GetVertexLight(0, 1, 1, worldX, worldY, worldZ, EAST));
			AddToBuffer(textureID, x + 0, y + 1, z + 0, 1, 1, World::GetInstance().GetVertexLight(0, 1, 0, worldX, worldY, worldZ, EAST));
			AddToBuffer(textureID, x + 0, y + 0, z + 0, 1, 0, World::GetInstance().GetVertexLight(0, 0, 0, worldX, worldY, worldZ, EAST));
		}
		else if (face == WEST)
		{
			AddToBuffer(textureID, x + 1, y + 0, z + 0, 0, 0, World::GetInstance().GetVertexLight(1, 0, 0, worldX, worldY, worldZ, WEST));
			AddToBuffer(textureID, x + 1, y + 1, z + 0, 0, 1, World::GetInstance().GetVertexLight(1, 1, 0, worldX, worldY, worldZ, WEST));
			AddToBuffer(textureID, x + 1, y + 1, z + 1, 1, 1, World::GetInstance().GetVertexLight(1, 1, 1, worldX, worldY, worldZ, WEST));
			AddToBuffer(textureID, x + 1, y + 0, z + 1, 1, 0, World::GetInstance().GetVertexLight(1, 0, 1, worldX, worldY, worldZ, WEST));
		}
		else if (face == TOP)
		{
			
			AddToBuffer(textureID, x + 0, y + 0, z + 1, 0, 0, World::GetInstance().GetVertexLight(0, 0, 1, worldX, worldY, worldZ, TOP));
			AddToBuffer(textureID, x + 0, y + 0, z + 0, 0, 1, World::GetInstance().GetVertexLight(0, 0, 0, worldX, worldY, worldZ, TOP));
			AddToBuffer(textureID, x + 1, y + 0, z + 0, 1, 1, World::GetInstance().GetVertexLight(1, 0, 0, worldX, worldY, worldZ, TOP));
			AddToBuffer(textureID, x + 1, y + 0, z + 1, 1, 0, World::GetInstance().GetVertexLight(1, 0, 1, worldX, worldY, worldZ, TOP));
		}
		else if (face == BOTTOM)
		{
			
			AddToBuffer(textureID, x + 0, y + 1, z + 0, 0, 0, World::GetInstance().GetVertexLight(0, 1, 0, worldX, worldY, worldZ, BOTTOM));
			AddToBuffer(textureID, x + 0, y + 1, z + 1, 0, 1, World::GetInstance().GetVertexLight(0, 1, 1, worldX, worldY, worldZ, BOTTOM));
			AddToBuffer(textureID, x + 1, y + 1, z + 1, 1, 1, World::GetInstance().GetVertexLight(1, 1, 1, worldX, worldY, worldZ, BOTTOM));
			AddToBuffer(textureID, x + 1, y + 1, z + 0, 1, 0, World::GetInstance().GetVertexLight(1, 1, 0, worldX, worldY, worldZ, BOTTOM));
		}
		else if (face == NORTH)
		{
			AddToBuffer(textureID, x + 0, y + 0, z + 0, 0, 0, World::GetInstance().GetVertexLight(0, 0, 0, worldX, worldY, worldZ, NORTH));
			AddToBuffer(textureID, x + 0, y + 1, z + 0, 0, 1, World::GetInstance().GetVertexLight(0, 1, 0, worldX, worldY, worldZ, NORTH));
			AddToBuffer(textureID, x + 1, y + 1, z + 0, 1, 1, World::GetInstance().GetVertexLight(1, 1, 0, worldX, worldY, worldZ, NORTH));
			AddToBuffer(textureID, x + 1, y + 0, z + 0, 1, 0, World::GetInstance().GetVertexLight(1, 0, 0, worldX, worldY, worldZ, NORTH));
		}
		else if (face == SOUTH)
		{
			
			AddToBuffer(textureID, x + 1, y + 0, z + 1, 0, 0, World::GetInstance().GetVertexLight(1, 0, 1, worldX, worldY, worldZ, SOUTH));
			AddToBuffer(textureID, x + 1, y + 1, z + 1, 0, 1, World::GetInstance().GetVertexLight(1, 1, 1, worldX, worldY, worldZ, SOUTH));
			AddToBuffer(textureID, x + 0, y + 1, z + 1, 1, 1, World::GetInstance().GetVertexLight(0, 1, 1, worldX, worldY, worldZ, SOUTH));
			AddToBuffer(textureID, x + 0, y + 0, z + 1, 1, 0, World::GetInstance().GetVertexLight(0, 0, 1, worldX, worldY, worldZ, SOUTH));
		}
		m_IndexCount += 6;
	}

	void Chunk::End()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Chunk::DrawAll()
	{
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

		//glBindVertexArray(0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Chunk::Bind()
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}

	void Chunk::DrawRange(GLuint start, GLuint end)
	{
		glDrawRangeElements(GL_TRIANGLES, start, m_IndexCount, end - start, GL_UNSIGNED_INT, NULL);
	}

	bType Chunk::BlockType(cPos x, cPos y, cPos z)
	{
		return blockType[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE];
	}

	bState Chunk::BlockState(cPos x, cPos y, cPos z)
	{
		return blockState[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE];
	}

	bLight Chunk::BlockLight(cPos x, cPos y, cPos z)
	{
		return blockLight[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE];
	}

	bTex Chunk::Texture(bType blockType, bState blockState, Face face)
	{
		if (blockType == STONE) return 1 + 0 * 16;

		if (blockType == GRASS) {
			if (face == TOP) return 0 + 0 * 16;
			if (face == BOTTOM) return 2 + 0 * 16;
			return 3;
		}
		if (blockType == DIRT) return 2 + 0 * 16;
		if (blockType == COBBLE) return 0 + 1 * 16;
		if (blockType == PLANKS) return 4 + 0 * 16;
		if (blockType == BEDROCK) return 1 + 1 * 16;
		if (blockType == SAND) return 2 + 1 * 16;
		if (blockType == GRAVEL) return 3 + 1 * 16;
		if (blockType == OAK_LOG)
		{
			if (face == TOP || face == BOTTOM) return 5 + 1 * 16;
			return 4 + 1 * 16;
		}

		

		return 8 + 10 * 16;
	}

	bool Chunk::Transparent(bType blockType)
	{
		return blockType == 0;
	}

	bool Chunk::BreaksOnPlacement(bType heldBlockType, bType targetBlockType)
	{
		return targetBlockType == 0;
	}

	bool Chunk::Transparent(cPos x, cPos y, cPos z)
	{
		return Transparent(blockType[x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE]);
	}
	
	bType Chunk::BreakBlock(int index)
	{
		if (index < 0 || index >= CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT) return 0;
		unsigned char previous = blockType[index];
		blockType[index] = 0;
		return previous;
	}

	bType Chunk::PlaceBlock(int index, bType block)
	{
		if (index < 0 || index >= CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT) return 0;
		unsigned char previous = blockType[index];
		if (previous == 0)
			blockType[index] = block;
		return previous;
	}
}