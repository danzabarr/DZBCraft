#pragma once

#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <string>

namespace DZB {

	static BYTE* LoadImage(const char* filename, GLsizei* width, GLsizei* height, unsigned int* bpp)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = nullptr;
		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		*bpp = FreeImage_GetBPP(dib);

		BYTE* pixels = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);

		return pixels;
	}
}