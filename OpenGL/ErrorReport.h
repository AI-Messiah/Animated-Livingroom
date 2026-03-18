#ifndef REPORT
#define REPORT

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <filesystem>

bool RequireFile(GLFWwindow* window, std::string path);

void CorruptedFile(GLFWwindow* window, const std::string& path);

void FailedShader(GLFWwindow* window, const std::string& message);

void InitFailure(const std::string& message);

#endif