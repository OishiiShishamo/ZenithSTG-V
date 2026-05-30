#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>


#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

#include "vec2d.h"

inline constexpr const char* kGameName = "ZenithSTG-V";
inline constexpr const char* kDefaultWindowTitle = "ZenithSTG-V";
inline const zenithstgv::Vec2D kDefaultResolution(1600, 900);



const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
inline constexpr bool enableValidationLayers = false;
#else
inline constexpr bool enableValidationLayers = true;
#endif



namespace zenithstgv {
	struct SDL_Deleter {
		void operator()(SDL_Window* window) const {
			if (window) {
				SDL_DestroyWindow(window);
			}
		}
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() 
				&& presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Application {
		public:
			using WindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;

			void run() {
				initWindow();
				initVulkan();
				mainLoop();
				cleanup();
			}

		private:
			WindowPtr window_;
			VkInstance instance_ = VK_NULL_HANDLE;
			VkSurfaceKHR surface_;

			VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
			VkDevice device_;

			VkQueue graphics_queue_;
			VkQueue present_queue_;

			VkSwapchainKHR swap_chain_;
			std::vector<VkImage> swap_chain_images_;
			VkFormat swap_chain_image_format_;
			VkExtent2D swap_chain_extent_;

			void initWindow() {
				if (!SDL_Init(SDL_INIT_VIDEO)) {
					std::cerr << "SDL_Init failed: "
						<< SDL_GetError()
						<< '\n';
					SDL_Quit();
					throw std::runtime_error(SDL_GetError());
				}

				window_.reset(
						SDL_CreateWindow(
							kDefaultWindowTitle,
							kDefaultResolution.GetX(),
							kDefaultResolution.GetY(),
							SDL_WINDOW_VULKAN
							)
						);

				if (!window_) {
					std::cerr << "SDL_CreateWindow failed: "
						<< SDL_GetError()
						<< '\n';
					SDL_Quit();
					throw std::runtime_error(SDL_GetError());
				}

				SDL_SetWindowPosition(
						window_.get(),
						SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED
						);
			}

			void initVulkan() {
				createInstance();
				createSurface();
				pickPhysicalDevice();
				createLogicalDevice();
				createSwapChain();
			}

			void mainLoop() {
				bool running = true;

				while (running) {
					SDL_Event event;

					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_EVENT_QUIT) {
							running = false;
						}
					}
				}
			}

			void cleanup() {
				if (swap_chain_ != VK_NULL_HANDLE) {
					vkDestroySwapchainKHR(device_, swap_chain_, nullptr);
				}

				if (device_ != VK_NULL_HANDLE) {
					vkDestroyDevice(device_, nullptr);
				}

				if (surface_ != VK_NULL_HANDLE) {
					vkDestroySurfaceKHR(instance_, surface_, nullptr);
					surface_ = VK_NULL_HANDLE;
				}

				if (instance_ != VK_NULL_HANDLE) {
					vkDestroyInstance(instance_, nullptr);
					instance_ = VK_NULL_HANDLE;
				}
				SDL_Quit();
			}

			void createInstance() {
				if (enableValidationLayers && !checkValidationLayerSupport()) throw std::runtime_error("validation layers requested, but not available!");

				VkApplicationInfo appInfo{};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = kGameName;
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = "ZenithSTG-V";
				appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.apiVersion = VK_API_VERSION_1_3;

				VkInstanceCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;

				std::uint32_t extension_count = 0;

				const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(
						&extension_count
						);

				if (!extensions) {
					std::cerr
						<< "SDL_Vulkan_GetInstanceExtensions failed: "
						<< SDL_GetError()
						<< '\n';

					SDL_Quit();
					throw std::runtime_error(SDL_GetError());
				}

				createInfo.enabledExtensionCount = extension_count;
				createInfo.ppEnabledExtensionNames = extensions;

				if (enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
					createInfo.ppEnabledLayerNames = validationLayers.data();

					createInfo.pNext = nullptr;
				} else {
					createInfo.enabledLayerCount = 0;

					createInfo.pNext = nullptr;
				}

				if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
					throw std::runtime_error("failed to create instance!");
				}
			}

			bool checkValidationLayerSupport() {
				uint32_t layerCount;
				vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

				std::vector<VkLayerProperties> availableLayers(layerCount);
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

			void createSurface() {
				if (!SDL_Vulkan_CreateSurface(window_.get(), instance_, nullptr, &surface_)) {
					throw std::runtime_error(SDL_GetError());
				}
			}


			void pickPhysicalDevice() {
				uint32_t deviceCount = 0;
				vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

				if (deviceCount == 0) {
					throw std::runtime_error("failed to find GPUs with Vulkan support!");
				}

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

				int bestScore = 0;
				for (const auto& device : devices) {
					int score = rateDeviceSuitability(device);
					if (score > bestScore) {
						bestScore = score;
						physical_device_ = device;
					}
				}

				if (physical_device_ == VK_NULL_HANDLE) {
					throw std::runtime_error("failed to find a suitable GPU!");
				}
			}

			void createLogicalDevice() {
				QueueFamilyIndices indices = findQueueFamilies(physical_device_);

				std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
				std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

				float queuePriority = 1.0f;
				for (uint32_t queueFamily : uniqueQueueFamilies) {
					VkDeviceQueueCreateInfo queueCreateInfo{};
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueCreateInfo.queueFamilyIndex = queueFamily;
					queueCreateInfo.queueCount = 1;
					queueCreateInfo.pQueuePriorities = &queuePriority;
					queueCreateInfos.push_back(queueCreateInfo);
				}

				VkPhysicalDeviceFeatures deviceFeatures{};

				VkDeviceCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

				createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
				createInfo.pQueueCreateInfos = queueCreateInfos.data();

				createInfo.pEnabledFeatures = &deviceFeatures;

				createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
				createInfo.ppEnabledExtensionNames = deviceExtensions.data();

				if (enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
					createInfo.ppEnabledLayerNames = validationLayers.data();
				} else {
					createInfo.enabledLayerCount = 0;
				}

				if (vkCreateDevice(physical_device_, &createInfo, nullptr, &device_) != VK_SUCCESS) {
					throw std::runtime_error("failed to create logical device!");
				}

				vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphics_queue_);
				vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &present_queue_);
			}

			int rateDeviceSuitability(VkPhysicalDevice device) {
				VkPhysicalDeviceProperties deviceProperties;
				VkPhysicalDeviceFeatures deviceFeatures;
				vkGetPhysicalDeviceProperties(device, &deviceProperties);
				vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

				QueueFamilyIndices indices = findQueueFamilies(device);
				if (!indices.isComplete()) return 0;

				int score = 0;

				if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
					score += 1000;

				score += deviceProperties.limits.maxImageDimension2D;

				bool extensionsSupported = checkDeviceExtensionSupport(device);

				bool swapChainAdequate = false;
				if (extensionsSupported) {
					SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
					swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
				}

				if (!extensionsSupported || !swapChainAdequate) return 0;
				return score;
			}

			void createSwapChain() {
				SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physical_device_);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

				uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
					imageCount = swapChainSupport.capabilities.maxImageCount;
				}

				VkSwapchainCreateInfoKHR createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = surface_;

				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

				QueueFamilyIndices indices = findQueueFamilies(physical_device_);
				uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

				if (indices.graphicsFamily != indices.presentFamily) {
					createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					createInfo.queueFamilyIndexCount = 2;
					createInfo.pQueueFamilyIndices = queueFamilyIndices;
				} else {
					createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
					createInfo.queueFamilyIndexCount = 0; // Optional
					createInfo.pQueueFamilyIndices = nullptr; // Optional
				}

				createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

				createInfo.presentMode = presentMode;
				createInfo.clipped = VK_TRUE;

				createInfo.oldSwapchain = VK_NULL_HANDLE;

				if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swap_chain_) != VK_SUCCESS) {
					throw std::runtime_error("failed to create swap chain!");
				}

				vkGetSwapchainImagesKHR(device_, swap_chain_, &imageCount, nullptr);
				swap_chain_images_.resize(imageCount);
				vkGetSwapchainImagesKHR(device_, swap_chain_, &imageCount, swap_chain_images_.data());

				swap_chain_image_format_ = surfaceFormat.format;
				swap_chain_extent_ = extent;
			}

			bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
				uint32_t extensionCount;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

				for (const auto& extension : availableExtensions) {
					requiredExtensions.erase(extension.extensionName);
				}

				return requiredExtensions.empty();
			}

			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
				SwapChainSupportDetails details;

				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
				}

				uint32_t presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, details.presentModes.data());
				}

				return details;
			}

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
				for (const auto& availableFormat : availableFormats) {
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
						return availableFormat;
					}
				}

				return availableFormats[0];
			}

			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
				for (const auto& availablePresentMode : availablePresentModes) {
					if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
						return availablePresentMode;
					}
				}

				return VK_PRESENT_MODE_FIFO_KHR;
			}

			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
				if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
					return capabilities.currentExtent;
				} else {
					int width = 0;
					int height = 0;
					SDL_GetWindowSizeInPixels(window_.get(), &width, &height);

					VkExtent2D actualExtent = {
						static_cast<uint32_t>(width),
						static_cast<uint32_t>(height)
					};

					actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
					actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

					return actualExtent;
				}
			}

			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
				QueueFamilyIndices indices;

				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

				for (uint32_t i = 0; i < queueFamilyCount; i++) {
					if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
						indices.graphicsFamily = i;

					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);
					if (presentSupport)
						indices.presentFamily = i;

					if (indices.isComplete()) break;
				}

				return indices;
			}
	};
}

int main()
{
	zenithstgv::Application app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	} 
	return EXIT_SUCCESS;
}
