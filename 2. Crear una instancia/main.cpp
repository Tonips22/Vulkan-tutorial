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

        // Instancia de Vulkan
        VkInstance instance;

        void initWindow() {
            // Iniciar GLFW
            glfwInit();

            // No se necesita OpenGL
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            // No se puede redimensionar
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            // Crear la ventana
            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Tutorial 2", nullptr, nullptr);
        } 

        void initVulkan() {
            createInstance();

        }

        void mainLoop() {
            // Bucle principal de la ventana hasta que se cierre
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }

        }

        void cleanup() {
            // Destruir la instancia de Vulkan
            vkDestroyInstance(instance, nullptr);

            // Limpiar la ventana
            glfwDestroyWindow(window);

            // Terminar GLFW
            glfwTerminate();
        }

        void createInstance() {
            // Información de la aplicación
            VkApplicationInfo appInfo{};
            // Tipo de estructura
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            // Nombre de la aplicación
            appInfo.pApplicationName = "Vulkan app";
            // Versión de la aplicación
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            // Nombre del motor
            appInfo.pEngineName = "No Engine";
            // Versión del motor
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            // Versión de la API de Vulkan
            appInfo.apiVersion = VK_API_VERSION_1_0;

            // Información de la creación de la instancia
            VkInstanceCreateInfo createInfo{};
            // Tipo de estructura
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            // Puntero a la información de la aplicación
            createInfo.pApplicationInfo = &appInfo;

            // Crear la instancia de Vulkan
            uint32_t glfwExtensionCount = 0;
            // Obtener las extensiones necesarias de GLFW
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            // Asignar las extensiones necesarias
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            // Asignar las capas de validación
            createInfo.enabledLayerCount = 0;

            // Crear la instancia de Vulkan
            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                throw runtime_error("Error al crear la instancia de Vulkan");
            }
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