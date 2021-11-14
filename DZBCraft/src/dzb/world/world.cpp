#include "world.h"
#include "../graphics/frustumculling.h"
#include "../graphics/draw.h"
namespace DZB
{
	Chunk* World::GetChunk(int id)
	{
		auto find = chunks.find(id);
		if (find == chunks.end())
			return nullptr;

		return find->second;
	}

	Chunk* World::GetChunk(int chunkX, int chunkZ)
	{
		return GetChunk((chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX);
	}

	bType World::BlockType(int x, int y, int z)
	{
		int chunkX = x >> 4;
		int chunkZ = z >> 4;

		int id = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;

		auto find = chunks.find(id);
		if (find == chunks.end())
			return 0;

		Chunk* chunk = find->second;
		
		x -= chunkX * CHUNK_SIZE;
		z -= chunkZ * CHUNK_SIZE;

		return chunk->BlockType(x, y, z);
	}

	void World::AddChunk(int chunkX, int chunkZ)
	{
		LOG(std::endl << "CH_ADD [" << chunkX << ", " << chunkZ << "]");
		int id = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;
		chunks[id] = new Chunk(chunkX, chunkZ);
	}

	void World::AddChunk(int chunkX, int chunkZ, bType blockID)
	{
		LOG(std::endl << "CH_ADD [" << chunkX << ", " << chunkZ << "]");
		int id = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;
		chunks[id] = new Chunk(chunkX, chunkZ, blockID);
	}

	void World::DeleteChunk(int chunkX, int chunkZ)
	{
		int id = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;
		auto find = chunks.find(id);
		if (find == chunks.end())
			return;

		LOG(std::endl << "CH_DEL [" << chunkX << ", " << chunkZ << "]");
		chunks.erase(find);
		delete find->second;
	}

	void World::DeleteChunk(Chunk* chunk)
	{
		if (chunk == nullptr)
			return;

		int id = chunk->GetID();
		auto find = chunks.find(id);
		if (find == chunks.end())
			return;

		LOG(std::endl << "CH_DEL [" << chunk->GetX() << ", " << chunk->GetZ() << "]");
		chunks.erase(find);
		delete chunk;
	}


	bLight World::GetLight(int worldX, int worldY, int worldZ)
	{
		int chunkX = worldX >> 4;
		int chunkZ = worldZ >> 4;
		Chunk* get = GetChunk(chunkX, chunkZ);
		if (get == nullptr)
			return 0;
		worldX -= chunkX * CHUNK_SIZE;
		worldZ -= chunkZ * CHUNK_SIZE;
		return get->BlockLight(worldX, worldY, worldZ);
	}

	bLight World::GetAverageLight(glm::ivec3 a, glm::ivec3 b, glm::ivec3 c, glm::ivec3 d)
	{
		bLight light = GetLight(a.x, a.y, a.z);

		//int transparentBlocks = 1;

		bool bT = Chunk::Transparent(BlockType(b.x, b.y, b.z));
		bool cT = Chunk::Transparent(BlockType(c.x, c.y, c.z));

		if (bT)
		{
			//transparentBlocks++;
		}

		if (cT)
		{
			//transparentBlocks++;
		}
		
		light += GetLight(b.x, b.y, b.z);
		light += GetLight(c.x, c.y, c.z);

		if (bT || cT)
		{
			if (Chunk::Transparent(BlockType(d.x, d.y, d.z)))
			{
				light += GetLight(d.x, d.y, d.z);
				//transparentBlocks++;
			}
		}

		return light / 4;
	}
	

	bLight World::GetVertexLight(int vX, int vY, int vZ, int worldX, int worldY, int worldZ, Face face)
	{
		if (face == EAST)
		{
			return GetAverageLight
			(
				glm::vec3(worldX - 1, worldY + 0, worldZ + 0),
				glm::vec3(worldX - 1, worldY + 0, worldZ - 1 + 2 * vZ),
				glm::vec3(worldX - 1, worldY - 1 + 2 * vY, worldZ + 0),
				glm::vec3(worldX - 1, worldY - 1 + 2 * vY, worldZ - 1 + 2 * vZ)
			);
			/*
			bLight light = GetLight(worldX - 1, worldY + 0, worldZ + 0);

			bLight bL = GetLight(worldX - 1, worldY + 0, worldZ - 1 + 2 * vZ);
			bLight cL = GetLight(worldX - 1, worldY - 1 + 2 * vY, worldZ + 0);

			light += bL;
			light += cL;

			bType bB = BlockType(worldX - 1, worldY + 0, worldZ - 1 + 2 * vZ);
			bType cB = BlockType(worldX - 1, worldY - 1 + 2 * vY, worldZ + 0);

			if (Chunk::Transparent(bB) || Chunk::Transparent(cB))
			{
				bLight dL = GetLight(worldX - 1, worldY - 1 + 2 * vY, worldZ - 1 + 2 * vZ);
				light += dL;
			}
			light /= 4;
			return light;
			*/
		}

		if (face == WEST)
		{
			return GetAverageLight
			(
				glm::vec3(worldX + 1, worldY + 0, worldZ + 0),
				glm::vec3(worldX + 1, worldY + 0, worldZ - 1 + 2 * vZ),
				glm::vec3(worldX + 1, worldY - 1 + 2 * vY, worldZ + 0),
				glm::vec3(worldX + 1, worldY - 1 + 2 * vY, worldZ - 1 + 2 * vZ)
			);
			/*
			bLight light = GetLight(worldX + 1, worldY + 0, worldZ + 0);

			bLight bL = GetLight(worldX + 1, worldY + 0, worldZ - 1 + 2 * vZ);
			bLight cL = GetLight(worldX + 1, worldY - 1 + 2 * vY, worldZ + 0);

			light += bL;
			light += cL;

			bType bB = BlockType(worldX + 1, worldY + 0, worldZ - 1 + 2 * vZ);
			bType cB = BlockType(worldX + 1, worldY - 1 + 2 * vY, worldZ + 0);

			if (Chunk::Transparent(bB) || Chunk::Transparent(cB))
			{
				bLight dL = GetLight(worldX + 1, worldY - 1 + 2 * vY, worldZ - 1 + 2 * vZ);
				light += dL;
			}
			light /= 4;
			return light;
			*/
		}

		if (face == TOP)
		{
			return GetAverageLight
			(
				glm::vec3(worldX + 0, worldY - 1, worldZ + 0),
				glm::vec3(worldX + 0, worldY - 1, worldZ - 1 + 2 * vZ),
				glm::vec3(worldX - 1 + 2 * vX, worldY - 1, worldZ + 0),
				glm::vec3(worldX - 1 + 2 * vX, worldY - 1, worldZ - 1 + 2 * vZ)
			);
			/*
			bLight light = GetLight(worldX + 0, worldY - 1, worldZ + 0);

			bLight bL = GetLight(worldX + 0, worldY - 1, worldZ - 1 + 2 * vZ);
			bLight cL = GetLight(worldX - 1 + 2 * vX, worldY - 1, worldZ + 0);

			light += bL;
			light += cL;

			bType bB = BlockType(worldX + 0, worldY - 1, worldZ - 1 + 2 * vZ);
			bType cB = BlockType(worldX - 1 + 2 * vX, worldY - 1, worldZ + 0);

			if (Chunk::Transparent(bB) || Chunk::Transparent(cB))
			{
				bLight dL = GetLight(worldX - 1 + 2 * vX, worldY - 1, worldZ - 1 + 2 * vZ);
				light += dL;
			}
			light /= 4;
			return light;
			*/
		}

		if (face == BOTTOM)
		{
			return GetAverageLight
			(
				glm::vec3(worldX + 0, worldY + 1, worldZ + 0),
				glm::vec3(worldX + 0, worldY + 1, worldZ - 1 + 2 * vZ),
				glm::vec3(worldX - 1 + 2 * vX, worldY + 1, worldZ + 0),
				glm::vec3(worldX - 1 + 2 * vX, worldY + 1, worldZ - 1 + 2 * vZ)
			);
			/*
			bLight light = GetLight(worldX + 0, worldY + 1, worldZ + 0);

			bLight bL = GetLight(worldX + 0, worldY + 1, worldZ - 1 + 2 * vZ);
			bLight cL = GetLight(worldX - 1 + 2 * vX, worldY + 1, worldZ + 0);

			light += bL;
			light += cL;

			bType bB = BlockType(worldX + 0, worldY + 1, worldZ - 1 + 2 * vZ);
			bType cB = BlockType(worldX - 1 + 2 * vX, worldY + 1, worldZ + 0);

			if (Chunk::Transparent(bB) || Chunk::Transparent(cB))
			{
				bLight dL = GetLight(worldX - 1 + 2 * vX, worldY + 1, worldZ - 1 + 2 * vZ);
				light += dL;
			}
			light /= 4;
			return light;
			*/
		}

		if (face == NORTH)
		{
			return GetAverageLight
			(
				glm::vec3(worldX + 0, worldY + 0, worldZ - 1),
				glm::vec3(worldX + 0, worldY - 1 + 2 * vY, worldZ - 1),
				glm::vec3(worldX - 1 + 2 * vX, worldY + 0, worldZ - 1),
				glm::vec3(worldX - 1 + 2 * vX, worldY - 1 + 2 * vY, worldZ - 1)
			);
			/*
			bLight light = GetLight(worldX + 0, worldY + 0, worldZ - 1);

			bLight bL = GetLight(worldX + 0, worldY - 1 + 2 * vY, worldZ - 1);
			bLight cL = GetLight(worldX - 1 + 2 * vX, worldY + 0, worldZ - 1);

			light += bL;
			light += cL;

			bType bB = BlockType(worldX + 0, worldY - 1 + 2 * vY, worldZ - 1);
			bType cB = BlockType(worldX - 1 + 2 * vX, worldY + 0, worldZ - 1);

			if (Chunk::Transparent(bB) || Chunk::Transparent(cB))
			{
				bLight dL = GetLight(worldX - 1 + 2 * vX, worldY - 1 + 2 * vY, worldZ - 1);
				light += dL;
			}
			light /= 4;
			return light;
			*/
		}

		if (face == SOUTH)
		{
			return GetAverageLight
			(
				glm::vec3(worldX + 0, worldY + 0, worldZ + 1),
				glm::vec3(worldX + 0, worldY - 1 + 2 * vY, worldZ + 1),
				glm::vec3(worldX - 1 + 2 * vX, worldY + 0, worldZ + 1),
				glm::vec3(worldX - 1 + 2 * vX, worldY - 1 + 2 * vY, worldZ + 1)
			);
			/*
			bLight light = GetLight(worldX + 0, worldY + 0, worldZ + 1);

			bLight bL = GetLight(worldX + 0, worldY - 1 + 2 * vY, worldZ + 1);
			bLight cL = GetLight(worldX - 1 + 2 * vX, worldY + 0, worldZ + 1);

			light += bL;
			light += cL;

			bType bB = BlockType(worldX + 0, worldY - 1 + 2 * vY, worldZ + 1);
			bType cB = BlockType(worldX - 1 + 2 * vX, worldY + 0, worldZ + 1);

			if (Chunk::Transparent(bB) || Chunk::Transparent(cB))
			{
				bLight dL = GetLight(worldX - 1 + 2 * vX, worldY - 1 + 2 * vY, worldZ + 1);
				light += dL;
			}
			light /= 4;
			return light;
			*/
		}

		return 0;
	}


	void World::CalculateLightDown(Chunk* chunk)
	{

		int x, y, z;
		int chunkIndex;
		bool hit;
		bType block;

		for (int index = 0; index < CHUNK_SIZE * CHUNK_SIZE; index++)
		{
			x = index % CHUNK_SIZE;
			z = index / CHUNK_SIZE;
			hit = false;
			for (y = 0; y < CHUNK_HEIGHT; y++)
			{
				chunkIndex = x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE;
				block = chunk->blockType[chunkIndex];
				if (!chunk->Transparent(block))
				{
					if (!hit) chunk->columnHeight[x + z * CHUNK_SIZE] = y;
					hit = true;
				}
				chunk->blockLight[chunkIndex] = hit ? 0 : 16;
			}
		}
	}

	void World::CalculateLightQueue(Chunk* chunk, std::queue<glm::ivec3>& queue)
	{
		int x, y, z;
		int chunkIndex;
		bool hit;
		bType block;
		int worldX, worldY, worldZ;
		int neighbourLight;
		int north, east, south, west;

		for (y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int index = 0; index < CHUNK_SIZE * CHUNK_SIZE; index++)
			{
				x = index % CHUNK_SIZE;
				z = index / CHUNK_SIZE;
				chunkIndex = x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE;

				worldX = x + chunk->GetX() * CHUNK_SIZE;
				worldZ = z + chunk->GetZ() * CHUNK_SIZE;

				if (chunk->blockLight[chunkIndex] == 0 && Chunk::Transparent(chunk->blockType[chunkIndex]))
				{
					neighbourLight = 0;
					
					east = GetLight(worldX, y, worldZ - 1);
					west = GetLight(worldX, y, worldZ + 1);
					north = GetLight(worldX - 1, y, worldZ);
					south = GetLight(worldX + 1, y, worldZ);

					if (east > neighbourLight) neighbourLight = east;
					if (west > neighbourLight) neighbourLight = west;
					if (north > neighbourLight) neighbourLight = north;
					if (south > neighbourLight) neighbourLight = south;

					if (neighbourLight > 1)
					{
						queue.push(glm::vec3(worldX, y, worldZ));
						chunk->blockLight[chunkIndex] = neighbourLight - 1;
					}
				}
			}
		}
	}

	void World::CalculateLightFlood(std::queue<glm::ivec3>& queue)
	{
		int worldX, worldY, worldZ;
		int light;
		while (queue.size() > 0)
		{


			worldX = queue.front().x;
			worldY = queue.front().y;
			worldZ = queue.front().z;
			queue.pop();

			light = GetLight(worldX, worldY, worldZ);
			
			BlockRef east(worldX - 1, worldY, worldZ);
			if (east.chunk != nullptr && east.chunk->blockLight[east.index] < light - 1 && Chunk::Transparent(east.chunk->blockType[east.index]))
			{
				east.chunk->blockLight[east.index] = light - 1;
				if (light - 1 > 1)
					queue.push(east.GetWorldPos());
			}

			BlockRef west(worldX + 1, worldY, worldZ);
			if (west.chunk != nullptr && west.chunk->blockLight[west.index] < light - 1 && Chunk::Transparent(west.chunk->blockType[west.index]))
			{
				west.chunk->blockLight[west.index] = light - 1;
				if (light - 1 > 1)
					queue.push(west.GetWorldPos());
			}

			BlockRef top(worldX, worldY - 1, worldZ);
			if (top.chunk != nullptr && top.chunk->blockLight[top.index] < light - 1 && Chunk::Transparent(top.chunk->blockType[top.index]))
			{
				top.chunk->blockLight[top.index] = light - 1;
				if (light - 1 > 1)
					queue.push(top.GetWorldPos());
			}

			BlockRef bottom(worldX, worldY + 1, worldZ);
			if (bottom.chunk != nullptr && bottom.chunk->blockLight[bottom.index] < light - 1 && Chunk::Transparent(bottom.chunk->blockType[bottom.index]))
			{
				bottom.chunk->blockLight[bottom.index] = light - 1;
				if (light - 1 > 1)
					queue.push(bottom.GetWorldPos());
			}

			BlockRef north(worldX, worldY, worldZ - 1);
			if (north.chunk != nullptr && north.chunk->blockLight[north.index] < light - 1 && Chunk::Transparent(north.chunk->blockType[north.index]))
			{
				north.chunk->blockLight[north.index] = light - 1;
				if (light - 1 > 1)
					queue.push(north.GetWorldPos());
			}

			BlockRef south(worldX, worldY, worldZ + 1);
			if (south.chunk != nullptr && south.chunk->blockLight[south.index] < light - 1 && Chunk::Transparent(south.chunk->blockType[south.index]))
			{
				south.chunk->blockLight[south.index] = light - 1;
				if (light - 1 > 1)
					queue.push(south.GetWorldPos());
			}
		}
	}

	void World::BufferChunk(Chunk* chunk)
	{
		int cX = chunk->GetX();
		int cZ = chunk->GetZ();

		Chunk* north = GetChunk(cX, cZ - 1);
		Chunk* south = GetChunk(cX, cZ + 1);
		Chunk* east = GetChunk(cX - 1, cZ);
		Chunk* west = GetChunk(cX + 1, cZ);

		int x, y, z;
		int blockX, blockY, blockZ;

		bType blockType;
		bState blockState;

		CalculateLightDown(chunk);
		std::queue<glm::ivec3> queue;
		CalculateLightQueue(chunk, queue);
		CalculateLightFlood(queue);

		bLight vertexLight[(CHUNK_SIZE + 1) * (CHUNK_HEIGHT + 1) * (CHUNK_SIZE + 1)];

		int averageLight;

		int n, nX, nY, nZ;

		for (int index = 0; index < (CHUNK_SIZE + 1) * (CHUNK_SIZE + 1) * (CHUNK_HEIGHT + 1); index++)
		{
			x = index % (CHUNK_SIZE + 1);
			z = (index / (CHUNK_SIZE + 1)) % (CHUNK_SIZE + 1);
			y = index / ((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));

			blockX = CHUNK_SIZE * cX + x;
			blockY = y;
			blockZ = CHUNK_SIZE * cZ + z;

			averageLight = 0;
			for (n = 0; n < 4; n++)
			{
				//nX = blockX - (n % 2);
				//nZ = blockZ - ((n / 2) % 2);
				//nY = blockY - (n / (2 * 2));
				nX = blockX - n % 2;
				nY = blockY - 1;
				nZ = blockZ - n / 2;
				averageLight += GetLight(nX, nY, nZ);
			}
			averageLight /= 4;

			vertexLight[index] = averageLight;
		}

		chunk->Begin();
		for (int index = 0; index < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; index++)
		{
			x = index % CHUNK_SIZE;
			z = (index / CHUNK_SIZE) % CHUNK_SIZE;
			y = index / (CHUNK_SIZE * CHUNK_SIZE);

			blockType = chunk->blockType[index];
			if (blockType == 0)
				continue;

			blockState = chunk->blockState[index];

			blockX = CHUNK_SIZE * cX + x;
			blockY = y;
			blockZ = CHUNK_SIZE * cZ + z;

			if ((x <= 0 && (east != nullptr && east->Transparent(CHUNK_SIZE - 1, y, z))) || (x > 0 && chunk->Transparent(x - 1, y, z)))
				chunk->DrawFace(Chunk::Texture(blockType, blockState, EAST), x, y, z, EAST);

			if ((x >= CHUNK_SIZE - 1 && (west != nullptr && west->Transparent(0, y, z))) || (x < CHUNK_SIZE - 1 && chunk->Transparent(x + 1, y, z)))
				chunk->DrawFace(Chunk::Texture(blockType, blockState, WEST), x, y, z, WEST);

			if (y <= 0 || (y > 0 && chunk->Transparent(x, y - 1, z)))
				chunk->DrawFace(Chunk::Texture(blockType, blockState, TOP), x, y, z, TOP);

			if (y < CHUNK_HEIGHT - 1 && chunk->Transparent(x, y + 1, z))
				chunk->DrawFace(Chunk::Texture(blockType, blockState, BOTTOM), x, y, z, BOTTOM);

			if ((z <= 0 && (north != nullptr && north->Transparent(x, y, CHUNK_SIZE - 1))) || (z > 0 && chunk->Transparent(x, y, z - 1)))
				chunk->DrawFace(Chunk::Texture(blockType, blockState, NORTH), x, y, z, NORTH);

			if ((z >= CHUNK_SIZE - 1 && (south != nullptr && south->Transparent(x, y, 0))) || (z < CHUNK_SIZE - 1 && chunk->Transparent(x, y, z + 1)))
				chunk->DrawFace(Chunk::Texture(blockType, blockState, SOUTH), x, y, z, SOUTH);

		}
		chunk->End();
	}



	void World::BufferChunks()
	{
		Chunk* chunk;
		
		Chunk* north;
		Chunk* south;
		Chunk* east;
		Chunk* west;

		int cX, cZ;
		cPos x, y, z;
		int blockX, blockY, blockZ;

		bType blockType;
		bState blockState;

		bLight vertexLight[(CHUNK_SIZE + 1) * (CHUNK_HEIGHT + 1) * (CHUNK_SIZE + 1)];

		int averageLight;

		int n, nX, nY, nZ;

		for (auto& it : chunks)
			CalculateLightDown(it.second);

		std::queue<glm::ivec3> queue;
		for (auto& it : chunks)
			CalculateLightQueue(it.second, queue);

		CalculateLightFlood(queue);

		for (auto& it : chunks)
		{
			int id = it.first;
			chunk = it.second;

			cX = chunk->GetX();// (id % CHUNK_MAX) - CHUNK_MAX / 2;
			cZ = chunk->GetZ();// (id / (CHUNK_MAX * CHUNK_MAX)) - CHUNK_MAX / 2;

			//if (FrustumCulling::CullChunk(frustum, cX, cY, cZ))
			//	continue;

			north = GetChunk(cX, cZ - 1);
			south = GetChunk(cX, cZ + 1);
			east = GetChunk(cX - 1, cZ);
			west = GetChunk(cX + 1, cZ);

			for (int index = 0; index < (CHUNK_SIZE + 1) * (CHUNK_SIZE + 1) * (CHUNK_HEIGHT + 1); index++)
			{
				x = index % (CHUNK_SIZE + 1);
				z = (index / (CHUNK_SIZE + 1)) % (CHUNK_SIZE + 1);
				y = index / ((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));

				blockX = CHUNK_SIZE * cX + x;
				blockY = y;
				blockZ = CHUNK_SIZE * cZ + z;

				averageLight = 0;
				for (n = 0; n < 4; n++)
				{
					//nX = blockX - (n % 2);
					//nZ = blockZ - ((n / 2) % 2);
					//nY = blockY - (n / (2 * 2));
					nX = blockX - n % 2;
					nY = blockY - 1;
					nZ = blockZ - n / 2;
					averageLight += GetLight(nX, nY, nZ);
				}
				averageLight /= 4;

				vertexLight[index] = averageLight;
			}

			chunk->Begin();

			for (int index = 0; index < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; index++)
			{
				x = index % CHUNK_SIZE;
				z = (index / CHUNK_SIZE) % CHUNK_SIZE;
				y = (index / (CHUNK_SIZE * CHUNK_SIZE));


				blockType = chunk->blockType[index];
				if (blockType == 0)
					continue;

				blockState = chunk->blockState[index];

				blockX = CHUNK_SIZE * cX + x;
				blockY = y;
				blockZ = CHUNK_SIZE * cZ + z;

				//	if (FrustumCulling::CullBlock(frustum, blockX, blockY, blockZ))
				//		continue;

				if ((x <= 0 && (east != nullptr && east->Transparent(CHUNK_SIZE - 1, y, z))) || (x > 0 && chunk->Transparent(x - 1, y, z)))
					chunk->DrawFace(Chunk::Texture(blockType, blockState, EAST), x, y, z, EAST);

				if ((x >= CHUNK_SIZE - 1 && (west != nullptr && west->Transparent(0, y, z))) || (x < CHUNK_SIZE - 1 && chunk->Transparent(x + 1, y, z)))
					chunk->DrawFace(Chunk::Texture(blockType, blockState, WEST), x, y, z, WEST);

				if (y <= 0 || (y > 0 && chunk->Transparent(x, y - 1, z)))
					chunk->DrawFace(Chunk::Texture(blockType, blockState, TOP), x, y, z, TOP);

				if (y < CHUNK_HEIGHT - 1 && chunk->Transparent(x, y + 1, z))
					chunk->DrawFace(Chunk::Texture(blockType, blockState, BOTTOM), x, y, z, BOTTOM);

				if ((z <= 0 && (north != nullptr && north->Transparent(x, y, CHUNK_SIZE - 1))) || (z > 0 && chunk->Transparent(x, y, z - 1)))
					chunk->DrawFace(Chunk::Texture(blockType, blockState, NORTH), x, y, z, NORTH);

				if ((z >= CHUNK_SIZE - 1 && (south != nullptr && south->Transparent(x, y, 0))) || (z < CHUNK_SIZE - 1 && chunk->Transparent(x, y, z + 1)))
					chunk->DrawFace(Chunk::Texture(blockType, blockState, SOUTH), x, y, z, SOUTH);
			}

			chunk->End();
		}
	}


	void World::RenderChunks(Shader* shader, Texture* texture, glm::vec4* frustum)
	{
		shader->enable();
		glActiveTexture(GL_TEXTURE0);
		texture->bind();

		for (auto& i : chunks)
		{
			Chunk* chunk = i.second;
			if (!FrustumCulling::CullChunk(frustum, chunk->GetX(), chunk->GetZ()))
			{
				//chunk->renderer->Bind();
				//chunk->renderer->DrawRange();
				shader->setUniform2f("chunk", glm::vec2(chunk->GetX(), chunk->GetZ()));

				
				chunk->Bind();
				chunk->DrawAll();
			}
		}

		shader->setUniform2f("chunk", glm::vec2());
	}

	void World::BlockUpdate(Chunk* chunk, cPos x, cPos y, cPos z)
	{
		BufferChunk(chunk);
		//if (x <= 0)
		//{
			Chunk* east = GetChunk(chunk->GetX() - 1, chunk->GetZ());
			if (east != nullptr)
				BufferChunk(east);


		//	if (z <= 0)
		//	{
				Chunk* northEast = GetChunk(chunk->GetX() - 1, chunk->GetZ() - 1);
				if (northEast != nullptr)
					BufferChunk(northEast);
		//	}
		//	else if (z >= CHUNK_SIZE - 1)
		//	{
				Chunk* southEast = GetChunk(chunk->GetX() - 1, chunk->GetZ() + 1);
				if (southEast != nullptr)
					BufferChunk(southEast);
		//	}

		//}
		//else if (x >= CHUNK_SIZE - 1)
		//{
			Chunk* west = GetChunk(chunk->GetX() + 1, chunk->GetZ());
			if (west != nullptr)
				BufferChunk(west);

		//	if (z <= 0)
		//	{
				Chunk* northWest = GetChunk(chunk->GetX() + 1, chunk->GetZ() - 1);
				if (northWest != nullptr)
					BufferChunk(northWest);
		//	}
		//	else if (z >= CHUNK_SIZE - 1)
		//	{
				Chunk* southWest = GetChunk(chunk->GetX() + 1, chunk->GetZ() + 1);
				if (southWest != nullptr)
					BufferChunk(southWest);
		//	}
		//}

		//if (z <= 0)
		//{
			Chunk* north = GetChunk(chunk->GetX(), chunk->GetZ() - 1);
			if (north != nullptr)
				BufferChunk(north);
		//}
		//else if (z >= CHUNK_SIZE - 1)
		//{
			Chunk* south = GetChunk(chunk->GetX(), chunk->GetZ() + 1);
			if (south != nullptr)
				BufferChunk(south);
		//}
	}


	bType World::BreakBlock(int worldX, int worldY, int worldZ)
	{
		int chunkX = worldX >> 4;
		int chunkZ = worldZ >> 4;
		int chunkID = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;
		
		auto find = chunks.find(chunkID);
		if (find == chunks.end())
			return 0;

		Chunk* chunk = find->second;

		int x = worldX - chunkX * CHUNK_SIZE;
		int y = worldY;
		int z = worldZ - chunkZ * CHUNK_SIZE;

		int chunkIndex = x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE;

		bType previous = chunk->BreakBlock(chunkIndex);
		if (previous == 0)
			return previous;

		BlockUpdate(chunk, x, y, z);

		return previous;
	}

	bType World::BreakBlock(RaycastHit hit)
	{
		Chunk* chunk = hit.chunk;
		cIndex chunkIndex = hit.index;

		cPos x = chunkIndex % CHUNK_SIZE;
		cPos z = (chunkIndex / CHUNK_SIZE) % CHUNK_SIZE;
		cPos y = (chunkIndex / (CHUNK_SIZE * CHUNK_SIZE));

		bType previous = chunk->BreakBlock(chunkIndex);
		if (previous == 0)
			return previous;

		BlockUpdate(chunk, x, y, z);

		return previous;
	}

	int World::PlaceBlock(bType heldBlock, int worldX, int worldY, int worldZ)
	{
		int chunkX = worldX >> 4;
		int chunkZ = worldZ >> 4;
		int chunkID = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;

		auto find = chunks.find(chunkID);
		if (find == chunks.end())
			return -1;

		Chunk* chunk = find->second;

		cPos x = worldX - chunkX * CHUNK_SIZE;
		cPos y = worldY;
		cPos z = worldZ - chunkZ * CHUNK_SIZE;

		cIndex chunkIndex = x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE;

		if (!Chunk::BreaksOnPlacement(heldBlock, chunk->blockType[chunkIndex]))
			return -1;

		bType previous = chunk->PlaceBlock(chunkIndex, heldBlock);

		BlockUpdate(chunk, x, y, z);

		return previous;
	}

	int World::PlaceBlock(bType heldBlock, RaycastHit hit)
	{
		glm::vec3 worldPos = hit.BreakPosition();

		switch (hit.face)
		{
		case EAST:
			return PlaceBlock(heldBlock, worldPos.x - 1, worldPos.y, worldPos.z);
		case WEST:
			return PlaceBlock(heldBlock, worldPos.x + 1, worldPos.y, worldPos.z);
		case TOP:
			return PlaceBlock(heldBlock, worldPos.x, worldPos.y - 1, worldPos.z);
		case BOTTOM:
			return PlaceBlock(heldBlock, worldPos.x, worldPos.y + 1, worldPos.z);
		case NORTH:
			return PlaceBlock(heldBlock, worldPos.x, worldPos.y, worldPos.z - 1);
		case SOUTH:
			return PlaceBlock(heldBlock, worldPos.x, worldPos.y, worldPos.z + 1);
		}

	}

	/**
 * Call the callback with (x,y,z,value,face) of all blocks along the line
 * segment from point 'origin' in vector direction 'direction' of length
 * 'radius'. 'radius' may be infinite.
 *
 * 'face' is the normal vector of the face of that block that was entered.
 * It should not be used after the callback returns.
 *
 * If the callback returns a true value, the traversal will be stopped.
 */
	bool World::RaycastBlock(const glm::vec3& origin, const glm::vec3& direction, float maxLength, int maxSteps, RaycastHit& out) {
		// From "A Fast Voxel Traversal Algorithm for Ray Tracing"
		// by John Amanatides and Andrew Woo, 1987
		// <http://www.cse.yorku.ca/~amana/research/grid.pdf>
		// <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
		// Extensions to the described algorithm:
		//   • Imposed a distance limit.
		//   • The face passed through to reach the current cube is provided to
		//     the callback.

		// The foundation of this algorithm is a parameterized representation of
		// the provided ray,
		//                    origin + t * direction,
		// except that t is not actually stored; rather, at any given point in the
		// traversal, we keep track of the *greater* t values which we would have
		// if we took a step sufficient to cross a cube boundary along that axis
		// (i.e. change the integer part of the coordinate) in the variables
		// tMaxX, tMaxY, and tMaxZ.

		// Cube containing origin point.
		int x = floor(origin.x);
		int y = floor(origin.y);
		int z = floor(origin.z);
		// Break out direction vector.
		float dx = direction.x;
		float dy = direction.y;
		float dz = direction.z;
		// Direction to increment x,y,z when stepping.
		int stepX = (int)signum(dx);
		int stepY = (int)signum(dy);
		int stepZ = (int)signum(dz);
		// See description above. The initial values depend on the fractional
		// part of the origin.
		float tMaxX = intbound(origin.x, dx);
		float tMaxY = intbound(origin.y, dy);
		float tMaxZ = intbound(origin.z, dz);
		// The change in t when taking a step (always positive).
		float tDeltaX = ((float)stepX) / dx;
		float tDeltaY = ((float)stepY) / dy;
		float tDeltaZ = ((float)stepZ) / dz;
		// Buffer for reporting faces to the callback.
		//glm::vec3 face = glm::vec3();
		Face face = EAST;

		// Avoids an infinite loop.
		if (dx == 0 && dy == 0 && dz == 0)
		{
			//std::cout << "Raycast in zero direction" << std::endl;
			
			return false;
		}
		//throw new RangeError("Raycast in zero direction!");

	// Rescale from units of 1 cube-edge to units of 'direction' so we can
	// compare with 't'.
		maxLength /= sqrt(dx * dx + dy * dy + dz * dz);

		int steps = 0;

		while (steps < maxSteps)/* ray has not gone past bounds of world */
		/*	((stepX > 0 ? x < wx : x >= 0) &&
			(stepY > 0 ? y < wy : y >= 0) &&
			(stepZ > 0 ? z < wz : z >= 0))
		*/

		{
			steps++;

			// Invoke the callback, unless we are not *yet* within the bounds of the
			// world.
			//if (!(x < 0 || y < 0 || z < 0 || x >= wx || y >= wy || z >= wz))
			//{


			int chunkX = x >> 4;
			int chunkY = (int)floor((float)y / CHUNK_HEIGHT);
			int chunkZ = z >> 4;

			int chunkID = (chunkX + CHUNK_MAX / 2) + (chunkZ + CHUNK_MAX / 2) * CHUNK_MAX;

			if (chunkY == 0) {
				auto find = chunks.find(chunkID);
				if (find != chunks.end())
				{
					Chunk* chunk = find->second;

					int relX = x - chunkX * CHUNK_SIZE;
					int relY = y;
					int relZ = z - chunkZ * CHUNK_SIZE;

					int chunkIndex = relX + relZ * CHUNK_SIZE + relY * CHUNK_SIZE * CHUNK_SIZE;

					int block = chunk->blockType[chunkIndex];

					if (block != 0)
					{
						//std::cout << "Steps: " << steps << std::endl;

						//TODO Collision tests with smaller hitboxes

						out.chunk = chunk;
						out.index = chunkIndex;
						out.face = face;
						return true;
					}
				}
			}

			

			//if (callback(x, y, z, blocks[x * wy * wz + y * wz + z], face))
			//	break;
		//}



		// tMaxX stores the t-value at which we cross a cube boundary along the
		// X axis, and similarly for Y and Z. Therefore, choosing the least tMax
		// chooses the closest cube boundary. Only the first case of the four
		// has been commented in detail.
			if (tMaxX < tMaxY) {
				if (tMaxX < tMaxZ) {
					if (tMaxX > maxLength)
					{
						//std::cout << "Out of range" << std::endl;
						return false;
					}
					// Update which cube we are now in.
					x += stepX;
					// Adjust tMaxX to the next X-oriented boundary crossing.
					tMaxX += tDeltaX;
					// Record the normal vector of the cube face we entered.
					//face[0] = -stepX;
					//face[1] = 0;
					//face[2] = 0;

					face = stepX > 0 ? EAST : WEST;

				}
				else {
					if (tMaxZ > maxLength)
					{
						//std::cout << "Out of range" << std::endl;
						return false;
					}
					z += stepZ;
					tMaxZ += tDeltaZ;
					//face[0] = 0;
					//face[1] = 0;
					//face[2] = -stepZ;

					face = stepZ > 0 ? NORTH : SOUTH;
				}
			}
			else {
				if (tMaxY < tMaxZ) {
					if (tMaxY > maxLength)
					{
						//std::cout << "Out of range" << std::endl;
						return false;
					}
					y += stepY;
					tMaxY += tDeltaY;
					//face[0] = 0;
					//face[1] = -stepY;
					//face[2] = 0;

					face = stepY > 0 ? TOP : BOTTOM;
				}
				else {
					// Identical to the second case, repeated for simplicity in
					// the conditionals.
					if (tMaxZ > maxLength)
					{
						//std::cout << "Out of range" << std::endl;
						return false;
					}
					z += stepZ;
					tMaxZ += tDeltaZ;
					//face[0] = 0;
					//face[1] = 0;
					//face[2] = -stepZ;

					face = stepZ > 0 ? NORTH : SOUTH;
				}
			}
		}

		//std::cout << "Too many steps" << std::endl;

		return false;
	}


	void World::DrawChunkBoundaries()
	{
		for (auto& i : chunks)
		{
			Chunk* chunk = i.second;

			Draw::WireCube
			(
				chunk->GetX() * CHUNK_SIZE,
				0,
				chunk->GetZ() * CHUNK_SIZE,
				chunk->GetX() * CHUNK_SIZE + CHUNK_SIZE,
				CHUNK_HEIGHT,
				chunk->GetZ() * CHUNK_SIZE + CHUNK_SIZE
			);
		}
	}
}
