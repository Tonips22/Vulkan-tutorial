#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>

using namespace std;

// Dimensiones de la ventana
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Capas de validación
const vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// Función para crear el depurador
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    // Cargar función
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    // Comprobar si la función está disponible
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// Función para destruir el depurador
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    // Cargar función
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    // Comprobar si la función está disponible
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

// Estructura para manejar los índices de la cola de comandos
struct QueueFamilyIndices {
    optional<uint32_t> graphicsFamily;

    // Verificar si los índices están completos
    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

// Clase principal de la aplicación Vulkan
class VulkanApplication {
public:
    // Método para ejecutar la aplicación
    void run() {
        initWindow(); // Inicializar la ventana
        initVulkan(); // Inicializar Vulkan
        mainLoop();   // Bucle principal
        cleanup();    // Liberar recursos
    }

private:
    GLFWwindow* window;                  // Ventana
    VkInstance instance;                 // Instancia de Vulkan
    VkDebugUtilsMessengerEXT debugMessenger; // Depurador de Vulkan

    // Inicializar la ventana
    void initWindow() {
        // Iniciar GLFW
        glfwInit();

        // Configurar GLFW para no crear un contexto OpenGL
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Crear la ventana
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    // Inicializar Vulkan
    void initVulkan() {
        createInstance();       // Crear la instancia de Vulkan
        setupDebugMessenger();  // Configurar el depurador
    }

    // Bucle principal de la aplicación
    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents(); // Procesar eventos de la ventana
        }
    }

    // Liberar recursos
    void cleanup() {
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyInstance(instance, nullptr); // Destruir la instancia de Vulkan
        glfwDestroyWindow(window);           // Destruir la ventana
        glfwTerminate();                      // Terminar GLFW
    }

    // Crear la instancia de Vulkan
    void createInstance() {
        // Verificar soporte para capas de validación
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw runtime_error("¡Capas de validación solicitadas, pero no disponibles!");
        }

        // Información de la aplicación
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Tipo de estructura
        appInfo.pApplicationName = "Vulkan app";           // Nombre de la aplicación
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // Versión de la aplicación
        appInfo.pEngineName = "No Engine";                   // Nombre del motor
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);     // Versión del motor
        appInfo.apiVersion = VK_API_VERSION_1_0;              // Versión de la API Vulkan

        // Información para crear la instancia
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // Tipo de estructura
        createInfo.pApplicationInfo = &appInfo;                    // Información de la aplicación

        // Obtener extensiones requeridas
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        // Crear la instancia
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw runtime_error("Error al crear la instancia de Vulkan!");
        }
    }

    // Configurar la información del depurador
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    // Configurar el depurador
    void setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw runtime_error("Error al configurar el depurador!");
        }
    }

    // Obtener extensiones requeridas
    vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    // Verificar soporte para capas de validación
    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    // Función de devolución de llamada del depurador
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        cerr << "capa de validación: " << pCallbackData->pMessage << endl;
        return VK_FALSE;
    }
};

// Función principal
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