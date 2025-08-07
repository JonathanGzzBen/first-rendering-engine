#version 450 core

const int MAX_NUM_POINT_LIGHTS = 100;

layout (location = 0) out vec4 fColor;

uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_diffuse_2;
uniform sampler2D texture_diffuse_3;

uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform vec3 point_light_positions[MAX_NUM_POINT_LIGHTS];
uniform vec3 viewPos;

in vec2 texPos;
in vec3 fragPos;
in vec3 normal;

vec3 ComputePointLight(vec3 light_position, vec3 view_dir, vec3 texColor) {
    vec3 light_dir = normalize(light_position - fragPos);
    float nDotL = dot(normal, light_dir);
    vec3 diffuse = diffuse_color * max(nDotL, 0.0) * vec3(texture(texture_diffuse_1, texPos)); // TODO: Use the rest of diffuse maps

    vec3 h = normalize(light_dir + view_dir);
    float nDotH = dot(normal, h);
    vec3 specular = specular_color * pow(max(nDotH, 0.0), 32); // TODO: Use specular map

    return diffuse + specular;
}

void main() {
    vec4 texColor = texture(texture_diffuse_1, texPos);
    vec3 ambient = ambient_color * texColor.rgb;
    vec3 view_dir = normalize(viewPos - fragPos);

    vec3 illumination = vec3(0.0);
    illumination += ambient;
    for (int i = 0; i < 1; i++) {
        vec3 light = ComputePointLight(point_light_positions[i], view_dir, texColor.rgb);
        illumination += light;
    }

    fColor = vec4(illumination, texColor.a);
}