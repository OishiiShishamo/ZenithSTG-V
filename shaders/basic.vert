#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in vec2 instancePos0;
layout(location = 4) in vec2 instancePos1;
layout(location = 5) in vec2 instancePos2;
layout(location = 6) in vec2 instancePos3;
layout(location = 7) in vec4 instanceColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 fragInstanceColor;

layout(push_constant) uniform PushConstants {
    float iTime;
} push;

void main() {
    vec2 positions[4] = vec2[](
        instancePos0,
        instancePos1,
        instancePos2,
        instancePos3
    );

    gl_Position = vec4(positions[gl_VertexIndex % 4], 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragInstanceColor = instanceColor;
}
