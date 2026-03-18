#include "ErrorReport.h"

bool RequireFile(GLFWwindow* window, std::string path)
{
    if (!std::filesystem::exists(path)) {
        HWND hwnd = glfwGetWin32Window(window);
        
        MessageBoxA(hwnd,
            ("Missing required file:\n" + path).c_str(),
            "Fatal Error",
            MB_ICONERROR | MB_OK);
        
        glfwSetWindowShouldClose(window, true);
        return false;
    }

    return true;
}

void CorruptedFile(GLFWwindow* window, const std::string& path)
{
    HWND hwnd = glfwGetWin32Window(window);

    MessageBoxA(hwnd,
        ("File Corrupted at:\n" + path).c_str(),
        "Fatal Error",
        MB_ICONERROR | MB_OK);

    glfwSetWindowShouldClose(window, true);
}

void FailedShader(GLFWwindow* window, const std::string& message) {
    HWND hwnd = glfwGetWin32Window(window);

    MessageBoxA(hwnd,
        ("Shader named: " + message + " failed to compile.").c_str(),
        "Shader Error",
        MB_ICONERROR | MB_OK);

    glfwSetWindowShouldClose(window, true);
}

void InitFailure(const std::string& message)
{
    MessageBoxA(NULL,
        message.c_str(),
        "Initialization Error",
        MB_ICONERROR | MB_OK);
}
