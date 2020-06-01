#include "Utility.h"
#include <algorithm>
#include <exception>
#include <Shlwapi.h>
#include <fstream>

std::string Utility::CurrentDirectory() {
	WCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, reinterpret_cast<LPSTR>(buffer));
	std::wstring currentDirectoryW(buffer);

	return std::string(currentDirectoryW.begin(), currentDirectoryW.end());
}

std::wstring Utility::ExecutableDirectory() {
	WCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, reinterpret_cast<LPSTR>(buffer), MAX_PATH);
	PathRemoveFileSpec(reinterpret_cast<LPSTR>(buffer));

	return std::wstring(buffer);
}

void Utility::GetFileName(const std::string& inputPath, std::string& filename) {
	std::string fullPath(inputPath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos) {
		filename = fullPath;
	} else {
		filename = fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
	}
}

void Utility::GetDirectory(const std::string& inputPath, std::string& directory) {
	std::string fullPath(inputPath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos) {
		directory = "";
	} else {
		directory = fullPath.substr(0, lastSlashIndex);
	}
}

void Utility::GetFileNameAndDirectory(const std::string& inputPath, std::string& directory, std::string& filename) {
	std::string fullPath(inputPath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos) {
		directory = "";
		filename = fullPath;
	} else {
		directory = fullPath.substr(0, lastSlashIndex);
		filename = fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
	}
}

void Utility::LoadBinaryFile(const std::wstring& filename, std::vector<char>& data) {
	std::ifstream file(reinterpret_cast<const char*>(filename.c_str()), std::ios::binary);
	if (file.bad()) {
		throw std::runtime_error("Could not open file.");
	}

	file.seekg(0, std::ios::end);
	UINT size = (UINT)file.tellg();

	if (size > 0) {
		data.resize(size);
		file.seekg(0, std::ios::beg);
		file.read(&data.front(), size);
	}

	file.close();
}

void Utility::ToWideString(const std::string& source, std::wstring& dest) {
	dest.assign(source.begin(), source.end());
}

std::wstring Utility::ToWideString(const std::string& source) {
	std::wstring dest;
	dest.assign(source.begin(), source.end());

	return dest;
}

void Utility::PathJoin(std::wstring& dest, const std::wstring& sourceDirectory, const std::wstring& sourceFile) {
	WCHAR buffer[MAX_PATH];

	PathCombine(reinterpret_cast<LPSTR>(buffer), reinterpret_cast<LPCSTR>(sourceDirectory.c_str()),
		reinterpret_cast<LPCSTR>(sourceFile.c_str()));
	dest = buffer;
}

void Utility::GetPathExtension(const std::wstring& source, std::wstring& dest) {
	dest = reinterpret_cast<const wchar_t*>(PathFindExtension(reinterpret_cast<LPCSTR>(source.c_str())));
}

GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {
	// ignore non-significant error/warning codes
	if (id == 131185) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}