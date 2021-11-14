#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

#include "defines.h"

#include "app/screen.h"
#include "app/input.h"

#include "utils/timer.h"

#include "world/world.h"
#include "world/player.h"

#include "graphics/draw.h"
#include "graphics/canvas.h"

#define GLT_IMPLEMENTATION
#include "utils/gltext.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = %x, severity = %x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

int main()
{
	using namespace DZB;

	std::cout << "Starting..." << std::endl;

	Screen::Create("DZBCraft", 800, 600);

	Shader blockShader("src/dzb/shaders/block.vert", "src/dzb/shaders/block.frag");
	Shader colorShader("src/dzb/shaders/color.vert", "src/dzb/shaders/color.frag");
	Shader spriteShader("src/dzb/shaders/sprite.vert", "src/dzb/shaders/sprite.frag");

	Texture blockTexture("src/dzb/textures/blocks.png", GL_NEAREST);
	Texture crossHairTexture("src/dzb/textures/crosshair.png", GL_LINEAR);

	Timer time;
	float updateTimer = 0;
	unsigned int updateFrames = 0;

	float fpsTimer = 0;
	unsigned int fpsFrames = 0;
	unsigned int fps = 0;

	Chunk::CreateIndexBuffer();

	int radius = 32;
	for (int z = -radius; z <= radius; z++)
		for (int x = -radius; x <= radius; x++)
			if (glm::distance(glm::vec2(), glm::vec2(x, z)) <= radius)
				World::GetInstance().AddChunk(x, z);
	

	World::GetInstance().BufferChunks();

	Screen::SetCursorMode(GLFW_CURSOR_DISABLED);
	
	Player player(glm::vec3(0.0f, 128.0f, 0.0f), 30.0f, 0.001f, 500.0f, glm::vec2(0.001f, 0.001f));
	Screen::AddListener(player.GetCamera());

	Canvas UICanvas(&spriteShader);
	Screen::AddListener(&UICanvas);

	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, nullptr);

	// Initialize glText
	gltInit();

	
	

	while (Screen::Active())
	{
		Screen::Clear();

	
	//Scene*********************************************
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		World::GetInstance().RenderChunks(&blockShader, &blockTexture, player.GetCamera()->GetFrustum());

		colorShader.enable();
		colorShader.setUniform4f("color", glm::vec4(0, 0, 0, 1));
		if (player.IsBlockInSights())
		{
			for (int i = 0; i < 10; i++)
				Draw::WireCube(player.BreakPosition(), 1.0025f + i * 0.0001f);
		}

		if (DEBUG)
		{
			colorShader.setUniform4f("color", glm::vec4(1, 1, 1, 1));
			World::GetInstance().DrawChunkBoundaries();
		}


		// Begin text drawing (this for instance calls glUseProgram)
		gltBeginDraw();

		// Draw any amount of text between begin and end
		gltColor(1.0f, 1.0f, 1.0f, 1.0f);

		std::string strings[3] =
		{
			"FPS: " + std::to_string(fps),
			"Chunks: " + std::to_string(World::GetInstance().ChunksCount()),
			"Faces: " + std::to_string(World::GetInstance().GetFacesBuffered()),
		};

		GLTtext* texts[3];

		for (int i = 0; i < 3; i++)
		{
			// Creating text
			GLTtext* text = gltCreateText();
			gltSetText(text, (char*)strings[i].data());
			gltDrawText2D(text, 0, i * 16, 1);
			texts[i] = text;
		}

		// Finish drawing text
		gltEndDraw();

		for (int i = 0; i < 3; i++)
			gltDeleteText(texts[i]);
	//**************************************************
	
	
	
	//User Interface************************************
		UICanvas.Begin();
		UICanvas.DrawSprite(&crossHairTexture, glm::vec3(Screen::GetWidth() / 2.0f - 10.0f, Screen::GetHeight() / 2.0f - 10.0f, 0), glm::vec2(20, 20));
		UICanvas.End();



	//**************************************************
		Screen::Update();

		fpsFrames++;
		updateFrames++;

		while (time.elapsed() - fpsTimer >= 1)
		{
			fpsTimer += 1;
			std::cout << fpsFrames << " fps" << std::endl;
			fps = fpsFrames;
			fpsFrames = 0;
		}

		while (time.elapsed() - updateTimer >= GAME_SPEED)
		{
			updateTimer += GAME_SPEED;
			updateFrames = 0;
			
			player.Update();

			blockShader.enable();
			blockShader.setUniformMat4("pr_matrix", player.GetCamera()->GetProjectionMatrix());
			blockShader.setUniformMat4("vw_matrix", player.GetCamera()->GetViewMatrix());

			colorShader.enable();
			colorShader.setUniformMat4("pr_matrix", player.GetCamera()->GetProjectionMatrix());
			colorShader.setUniformMat4("vw_matrix", player.GetCamera()->GetViewMatrix());
		}
	}

	

	// Destroy glText
	gltTerminate();

	return 0;
}


