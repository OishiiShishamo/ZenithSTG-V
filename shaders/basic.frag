#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragInstanceColor;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    float iTime;
} push;

void main() {
    vec4 col = texture(texSampler, fragTexCoord);
    outColor = col * fragInstanceColor;
}
