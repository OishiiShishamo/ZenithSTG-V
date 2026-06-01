#ifndef ZENITHSTGV_INCLUDE_VERTEX_H_
#define ZENITHSTGV_INCLUDE_VERTEX_H_

#include <array>

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

namespace zenithstgv {
struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static vk::VertexInputBindingDescription getBindingDescription() {
		return {0, sizeof(Vertex), vk::VertexInputRate::eVertex};
	}

	static std::array<vk::VertexInputAttributeDescription, 3>
	getAttributeDescriptions() {
		return {{
		    {
		        0,
		        0,
		        vk::Format::eR32G32Sfloat,
		        offsetof(Vertex, pos),
		    },
		    {
		        1,
		        0,
		        vk::Format::eR32G32B32Sfloat,
		        offsetof(Vertex, color),
		    },
		    {
		        2,
		        0,
		        vk::Format::eR32G32Sfloat,
		        offsetof(Vertex, texCoord),
		    },
		}};
	}
};
} // namespace zenithstgv

#endif
