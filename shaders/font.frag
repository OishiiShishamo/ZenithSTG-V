#version 450

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragInstanceColor;

layout(push_constant) uniform PushConstants {
    float iTime;
} push;

layout(location = 0) out vec4 outColor;

void main() {
    float d = texture(texSampler, fragTexCoord).r;

    float edge0 = 0.5 - 0.15;
    float edge1 = 0.5 + 0.15;

    float alpha = smoothstep(edge0, edge1, d);

    alpha = pow(alpha, 1.0 / 1.6);

    outColor = vec4(fragInstanceColor.rgb, alpha * fragInstanceColor.a);
}
