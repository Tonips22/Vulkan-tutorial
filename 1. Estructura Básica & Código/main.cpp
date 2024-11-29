#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace std;

class VulkanApplication {
    public:
        void run() {
            // Iniciar la ventana
            initWindow();

            // Iniciar Vulkan
            initVulkan();

            // Bucle principal
            mainLoop();

            // Limpiar Vulkan
            cleanup();
        }

    private:
        // Instancia de la ventana
        GLFWwindow* window;

        // Dimensiones de la ventana
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;

        void initWindow() {
            // Iniciar GLFW
            glfwInit();

            // No se necesita OpenGL
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            // No se puede redimensionar
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            // Crear la ventana
            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Tutorial 1", nullptr, nullptr);
        } 
        void initVulkan() {

        }

        void mainLoop() {
            // Bucle principal de la ventana hasta que se cierre
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }

        }

        void cleanup() {
            // Limpiar la ventana
            glfwDestroyWindow(window);

            // Terminar GLFW
            glfwTerminate();
        }
};

int main() {
    VulkanApplication app;

    try {
        app.run();
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}