#pragma once

#include "Common.h"
#include <glew.h>
#include <iostream>

class Utility {
public:
	static std::string CurrentDirectory();
	static std::wstring ExecutableDirectory();
	static void GetFileName(const std::string& inputPath, std::string& filename);
	static void GetDirectory(const std::string& inputPath, std::string& directory);
	static void GetFileNameAndDirectory(const std::string& inputPath, std::string& directory, std::string& filename);
	static void LoadBinaryFile(const std::wstring& filename, std::vector<char>& data);
	static void ToWideString(const std::string& source, std::wstring& dest);
	static std::wstring ToWideString(const std::string& source);
	static void PathJoin(std::wstring& dest, const std::wstring& sourceDirectory, const std::wstring& sourceFile);
	static void GetPathExtension(const std::wstring& source, std::wstring& dest);

private:
	Utility();
	Utility(const Utility& rhs);
	Utility& operator=(const Utility& rhs);
};
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);
GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 