#version 410 core
in vec3 vNormal;
in vec3 vFragPos;
out vec4 FragColor;

uniform vec3 uLightDir;
uniform vec3 uObjectColor;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(-uLightDir);

    float ambient = 0.3;
    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 result = uObjectColor * (ambient + diffuse * 0.7);
    FragColor = vec4(result, 1.0);
}