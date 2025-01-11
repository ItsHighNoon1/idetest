#include "render.h"

#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    uint32_t graphicsFamily;
    int complete;
} QueueFamilyIndices;

static inline int _createInstance();
static inline int _pickPhysicalDevice();
static inline QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);

static VkInstance instance;
static VkPhysicalDevice physicalDevice;

int render_init() {
    int rc = 0;
    rc |= _createInstance();
    rc |= _pickPhysicalDevice();
    return rc;
}

void render_destroy() {
    vkDestroyInstance(instance, NULL);
}

// 1000 lines of boilerplate go!

static inline int _createInstance() {
    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "idetest";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    return result != VK_SUCCESS;
}

static inline int _pickPhysicalDevice() {
    physicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        return -1;
    }

    VkPhysicalDevice* devices = malloc(deviceCount * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    int bestScore = -1;
    for (int deviceIdx = 0; deviceIdx < deviceCount; deviceIdx++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[deviceIdx], &deviceProperties);

        // Start with suitability checks
        QueueFamilyIndices indices = _findQueueFamilies(devices[deviceIdx]);
        if (indices.complete == 0) {
            continue;
        }

        // Then bias towards better hardware
        int deviceScore = 0;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            deviceScore += 10000;
        }
        deviceScore += deviceProperties.limits.maxStorageBufferRange;

        if (deviceScore > bestScore) {
            physicalDevice = devices[deviceIdx];
        }
    }

    free(devices);
    return bestScore < 0;
}

static inline QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices = {0};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties* queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    int foundGraphicsFamily = 0;
    for (int queueFamilyIdx = 0; queueFamilyIdx < queueFamilyCount; queueFamilyIdx++) {
        if (queueFamilies[queueFamilyIdx].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = queueFamilyIdx;
            foundGraphicsFamily = 1;
        }
    }

    free(queueFamilies);

    int foundAllFamilies = 1;
    foundAllFamilies = foundAllFamilies && foundGraphicsFamily;
    indices.complete = foundAllFamilies;

    return indices;
}