#pragma once

// System files
#include <string>
#include <memory>

// necessary for proper includes
#include "Platform.h"

// Engine dependencies
#include "Graphics/GLAPI.h"

#include <glm/glm.hpp>

// Engine files
#include "EventDispatcher.h"
#include "Surface.h"
#include "Input.h"

struct WindowResizeEventData {
    int width;
    int height;
};

class Window : public Surface {
private:
    GLFWwindow* handle;

    static uint32_t WINDOW_COUNT;

    static void glfwErrorCallback(int e, const char *d);
    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
    EventDispatcher<WindowResizeEventData> windowResizeEvent;

    std::shared_ptr<Input> input;
public:
    Window(const std::string title, int width, int height);
    ~Window();

    // returns if the should close flag of the window has been set (e.g. if the "close" button is pressed)
    bool ShouldClose();
    void SetShouldClose();
    
    void Swap();
    
    // returns the size of the window
    glm::ivec2 GetSize();

    // UpdateSize calls glViewport to adjust it to the window's size.
    void UpdateSize();

    GLFWwindow* GetHandle() { return handle; };

    auto& GetWindowResizeHandler() { return windowResizeEvent; };
};
