#pragma once
#include <GLFW/glfw3.h>

namespace DZB
{
	class Draw
	{
	public:

		static void Line(glm::vec3 start, glm::vec3 end)
		{
			glBegin(GL_LINES);
			glVertex3f(start.x, start.y, start.z);
			glVertex3f(end.x, end.y, end.z);
			glEnd();
		}

		static void WireCube(glm::vec3 position, float size)
		{
			Draw::WireCube(position.x, position.y, position.z, size);
		}

		static void WireCube(float x, float y, float z, float size)
		{
			x += 0.5f;
			y += 0.5f;
			z += 0.5f;

			glBegin(GL_LINES);
			glVertex3f(x - size / 2, y - size / 2, z - size / 2);
			glVertex3f(x + size / 2, y - size / 2, z - size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + size / 2, y - size / 2, z - size / 2);
			glVertex3f(x + size / 2, y - size / 2, z + size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + size / 2, y - size / 2, z + size / 2);
			glVertex3f(x - size / 2, y - size / 2, z + size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x - size / 2, y - size / 2, z + size / 2);
			glVertex3f(x - size / 2, y - size / 2, z - size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x - size / 2, y + size / 2, z - size / 2);
			glVertex3f(x + size / 2, y + size / 2, z - size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + size / 2, y + size / 2, z - size / 2);
			glVertex3f(x + size / 2, y + size / 2, z + size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + size / 2, y + size / 2, z + size / 2);
			glVertex3f(x - size / 2, y + size / 2, z + size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x - size / 2, y + size / 2, z + size / 2);
			glVertex3f(x - size / 2, y + size / 2, z - size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x - size / 2, y - size / 2, z - size / 2);
			glVertex3f(x - size / 2, y + size / 2, z - size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + size / 2, y - size / 2, z - size / 2);
			glVertex3f(x + size / 2, y + size / 2, z - size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + size / 2, y - size / 2, z + size / 2);
			glVertex3f(x + size / 2, y + size / 2, z + size / 2);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x - size / 2, y - size / 2, z + size / 2);
			glVertex3f(x - size / 2, y + size / 2, z + size / 2);
			glEnd();
		}

		static void WireCube(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
		{

			glBegin(GL_LINES);
			glVertex3f(minX, minY, minZ);
			glVertex3f(maxX, minY, minZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(maxX, minY, minZ);
			glVertex3f(maxX, minY, maxZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(maxX, minY, maxZ);
			glVertex3f(minX, minY, maxZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(minX, minY, maxZ);
			glVertex3f(minX, minY, minZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(minX, maxY, minZ);
			glVertex3f(maxX, maxY, minZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(maxX, maxY, minZ);
			glVertex3f(maxX, maxY, maxZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(maxX, maxY, maxZ);
			glVertex3f(minX, maxY, maxZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(minX, maxY, maxZ);
			glVertex3f(minX, maxY, minZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(minX, minY, minZ);
			glVertex3f(minX, maxY, minZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(maxX, minY, minZ);
			glVertex3f(maxX, maxY, minZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(maxX, minY, maxZ);
			glVertex3f(maxX, maxY, maxZ);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(minX, minY, maxZ);
			glVertex3f(minX, maxY, maxZ);
			glEnd();
		}
	};
	
}