#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D textureSampler;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(vec3(1.0, -1.0, -1.0));
    float diff = max(dot(norm, lightDir), 0.0);

    vec4 texColor = texture(textureSampler, fragTexCoord);
    outColor = vec4(diff * texColor.rgb, texColor.a);
}
