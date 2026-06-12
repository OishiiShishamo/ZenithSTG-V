#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragInstanceColor;

layout(push_constant) uniform PushConstants {
    float iTime;
} push;

layout(location = 0) out vec4 outColor;

void main() {
	vec2 dx = dFdx(fragTexCoord);
    vec2 dy = dFdy(fragTexCoord);
    vec4 col = textureGrad(texSampler, fragTexCoord, dx, dy);

	vec3 linRGB = col.rgb * col.rgb;
    float luma = dot(linRGB, vec3(0.299, 0.587, 0.114));

    float mask = smoothstep(0.92, 0.98, luma);

    vec3 rgb = mix(col.rgb, col.rgb * fragInstanceColor.rgb, mask);

	float a = col.a * fragInstanceColor.a;
    outColor = vec4(rgb, a);
}
