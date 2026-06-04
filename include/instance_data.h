#pragma once
#ifndef ZENITHSTGV_INCLUDE_INSTANCE_DATA_H_
#define ZENITHSTGV_INCLUDE_INSTANCE_DATA_H_

#include <array>

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "color.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"

namespace zenithstgv {

enum BlendMode {
	kNormal = 0,
	kAdd,
	kSubtract,
	kMultiply,
};

struct InstanceData {
	glm::vec2 pos[4];
	glm::vec4 color;
	glm::vec4 uv;
	float blend_pal;

	static vk::VertexInputBindingDescription getBindingDescription() {
		return {1, sizeof(InstanceData), vk::VertexInputRate::eInstance};
	}

	static std::array<vk::VertexInputAttributeDescription, 7>
	getAttributeDescriptions() {
		return {{
		    {3, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[0])},
		    {4, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[1])},
		    {5, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[2])},
		    {6, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceData, pos[3])},
		    {7, 1, vk::Format::eR32G32B32A32Sfloat,
		     offsetof(InstanceData, color)},
		    {8, 1, vk::Format::eR32G32B32A32Sfloat, offsetof(InstanceData, uv)},
		    {9, 1, vk::Format::eR32Sfloat, offsetof(InstanceData, blend_pal)},
		}};
	}
};

struct Sprite {
	Vec2D pos[4];
	Color color;
	BlendMode blend_mode = BlendMode::kNormal;
	std::string uv_key;
	float blend_pal = 1.0;

	InstanceData toInstanceData(const AtlasEntry &e) const {
		InstanceData d;

		for (int i = 0; i < 4; i++) {
			d.pos[i] = glm::vec2(pos[i].GetX(), pos[i].GetY());
		}

		d.color =
		    glm::vec4(color.GetR(), color.GetG(), color.GetB(), color.GetA());

		d.uv = glm::vec4(e.uv.u0, e.uv.v0, e.uv.u1, e.uv.v1);

		d.blend_pal = blend_pal;

		return d;
	}
};

} // namespace zenithstgv
#endif
