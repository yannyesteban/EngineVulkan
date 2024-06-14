#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);
    outColor = texColor * vec4(fragColor, 1.0);

}


/*#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Dirección de la luz
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * fragColor;
    vec3 ambient = 0.1 * fragColor; // Luz ambiental
    
    //outColor = vec4(ambient + diffuse, 1.0);
    outColor = vec4(1.0, 0.2, 0.3, 1.0);
}*/
