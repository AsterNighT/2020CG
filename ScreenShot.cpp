#include "ScreenShot.h"

#include<glew.h>
#include<cstdlib>
#include<cstdio>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

bool ScreenShot(int height, int width, const char* filename) {
	//printf("%d %d\n", wHeight, wWidth);

	const size_t pitch = (width * 3 + 3) & ~3; // DWORD aligned line
	const size_t len = pitch * height * sizeof(GLbyte);
	GLbyte* buffer = (GLbyte*)malloc(len);
	if (!buffer)
	{
		fprintf(stderr, "buffer alloc failed!\n");
		return false;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, buffer);

	FILE* file = fopen(filename, "wb");
	if (!file)
	{
		fprintf(stderr, "can't open %s\n", filename);
		free(buffer);
		return false;
	}

	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;

	file_header.bfType = (WORD)('B' | 'M' << 8); // Windows BMP file tag
	printf("OFF = %d, %d\n", sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
	printf("%d %d\n", sizeof(file_header.bfType), sizeof(file_header.bfSize));
	file_header.bfSize = len + 54;// sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + len;
	file_header.bfReserved1 = 0;
	file_header.bfReserved2 = 0;
	file_header.bfOffBits = 54; // sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	info_header.biSize = sizeof(BITMAPINFOHEADER);
	info_header.biWidth = width;
	info_header.biHeight = height;
	info_header.biPlanes = 1;
	info_header.biBitCount = 24;
	info_header.biCompression = 0; // BI_RGB, An uncompressed format.
	info_header.biSizeImage = len; // size in bytes. This may be set to zero for BI_RGB bitmaps
	info_header.biXPelsPerMeter = 0;
	info_header.biYPelsPerMeter = 0;
	info_header.biClrUsed = 0;
	info_header.biClrImportant = 0;

	fwrite(&file_header, sizeof(file_header), 1, file);
	fwrite(&info_header, sizeof(info_header), 1, file);
	fwrite(buffer, len, 1, file);
	fclose(file);

	free(buffer);
	return true;
}

