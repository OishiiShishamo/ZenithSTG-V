#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    float iTime;
} push;

vec4 hsv2rgb(vec4 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return vec4(c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y), c.w);
}

void main() {
	vec4 col = texture(texSampler, fragTexCoord);
	col += col * hsv2rgb(vec4(push.iTime * 0.25, 1., 1.5, 1.));
	col /= hsv2rgb(vec4(fract(fragTexCoord.x * fragTexCoord.y + (push.iTime / 4.0)), 1., 1., 1.));
    outColor = col;
}
