#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>

// Dimensiones de la ventana
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Capas de validación
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

class VulkanApplication {
public:
    void run() {
        // Iniciar Ventana
        initWindow();
        // Iniciar Vulkan
        initVulkan();
        // Bucle principal
        mainLoop();
        // Limpiar
        cleanup();
    }

private:
    // Ventana
    GLFWwindow* window;

    // Instancia de Vulkan
    VkInstance instance;
    // Depurador
    VkDebugUtilsMessengerEXT debugMessenger;

    void initWindow() {
        // Iniciar GLFW
        glfwInit();

        // No crear contexto OpenGL
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Crear ventana
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        setupDebugMessenger();
    }

    void mainLoop() {
        // Bucle principal
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // Limpiar
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        // Destruir instancia
        vkDestroyInstance(instance, nullptr);

        // Cerrar ventana
        glfwDestroyWindow(window);

        // Terminar GLFW
        glfwTerminate();
    }

    void createInstance() {
        // Comprobar capas de validación
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("¡Capas de validación solictadas, pero no disponibles!");
        }

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

        // Crear información de la instancia
        VkInstanceCreateInfo createInfo{};
        // Tipo de estructura
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        // Información de la aplicación
        createInfo.pApplicationInfo = &appInfo;

        // Extensiones
        auto extensions = getRequiredExtensions();
        // Número de extensiones
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        // Extensiones
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Capas de validación
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        // Número de capas de validación
        if (enableValidationLayers) {
            // Número de capas de validación
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            // Capas de validación
            createInfo.ppEnabledLayerNames = validationLayers.data();

            // Configurar depurador
            populateDebugMessengerCreateInfo(debugCreateInfo);
            // Puntero a la estructura de depurador
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            // Número de capas de validación
            createInfo.enabledLayerCount = 0;

            // Puntero nulo
            createInfo.pNext = nullptr;
        }

        // Crear instancia
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        // Tipo de estructura
        createInfo = {};
        // Tipo de estructura
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        // Severidad de los mensajes
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        // Tipo de mensajes
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        // Puntero a la función de devolución de llamada
        createInfo.pfnUserCallback = debugCallback;
    }

    // Configurar depurador
    void setupDebugMessenger() {
        // No se requiere depurador
        if (!enableValidationLayers) return;

        // Configurar depurador
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        // Tipo de estructura
        populateDebugMessengerCreateInfo(createInfo);

        // Crear depurador
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("Error al configurar el depurador!");
        }
    }

    // Extensiones requeridas
    std::vector<const char*> getRequiredExtensions() {
        // Extensiones de GLFW
        uint32_t glfwExtensionCount = 0;
        // Extensiones de GLFW
        const char** glfwExtensions;
        // Obtener extensiones de GLFW
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Extensiones requeridas
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // Capas de validación
        if (enableValidationLayers) {
            // Agregar capa de validación
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    // Comprobar capas de validación
    bool checkValidationLayerSupport() {
        // Número de capas
        uint32_t layerCount;
        // Obtener número de capas
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        // Capas disponibles
        std::vector<VkLayerProperties> availableLayers(layerCount);
        // Obtener capas
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Comprobar capas
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

    // Función de devolución de llamada
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "capa de validación: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
};

int main() {
    VulkanApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}