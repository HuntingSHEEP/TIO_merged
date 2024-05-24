#version 450
layout(binding = 1) uniform sampler2D texSampler;

struct PointLight {
    vec4 position; //ignore w
    vec4 color; // w is intensity
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    PointLight pointLights[100];
    int numberOfLights;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 fragPosition;
layout(location = 4) in float number;

layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(material * I, 1.0);
    outColor = vec4(fragColor, 1.0) * texture(texSampler, fragTexCoord);
}
