#pragma once
#ifndef ZENITHSTGV_INCLUDE_INSTANCE_DATA_H_
#define ZENITHSTGV_INCLUDE_INSTANCE_DATA_H_

#include <array>

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "color.h"
#include "vec2d.h"

namespace zenithstgv {

enum class BlendMode {
	kNormal = 0,
	kAdd,
	kSubtract,
	kMultiply,
};

struct InstanceData {
	glm::vec2 pos[4];
	glm::vec4 color;

	static vk::VertexInputBindingDescription getBindingDescription() {
		return {1, sizeof(InstanceData), vk::VertexInputRate::eInstance};
	}

	static std::array<vk::VertexInputAttributeDescription, 5>
	getAttributeDescriptions() {
		return {{
		    {3, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[0])},
		    {4, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[1])},
		    {5, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[2])},
		    {6, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[3])},
		    {7, 1, vk::Format::eR32G32B32A32Sfloat,
		     offsetof(InstanceData, color)},
		}};
	}
};

struct Sprite {
	Vec2D pos[4];
	Color color;
	BlendMode blend_mode = BlendMode::kNormal;

	InstanceData toInstanceData() const {
		InstanceData d;
		for (int i = 0; i < 4; i++) {
			d.pos[i] = {static_cast<float>(pos[i].GetX()),
			            static_cast<float>(pos[i].GetY())};
		}
		d.color = {color.GetR(), color.GetG(), color.GetB(), color.GetA()};
		return d;
	}
};

} // namespace zenithstgv
#endif
