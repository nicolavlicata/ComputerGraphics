#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
in vec3 norm;
in vec3 fragPos;
// We always define a fragment color that we output.
out vec4 fragColor;

struct PointLight {
    vec3 color;
    vec3 position;
    float ambientIntensity;
    float specularIntensity;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[1];
uniform sampler2D tex;
uniform mat4 view;

void main() {
  vec4 diffuseColor = texture(tex, texCoords);

  vec3 light = vec3(0.0, 0.0, 0.0);
  vec3 ambientLight = pointLights[0].ambientIntensity * pointLights[0].color;

  vec3 lightDir = normalize(pointLights[0].position - fragPos);
  float diffImpact = max(dot(norm, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * pointLights[0].color;
  vec3 viewPos = vec3(0.0, 0.0, 0.0);
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specularLight = pointLights[0].specularIntensity * spec * pointLights[0].color;
  float distance = length(pointLights[0].position - fragPos);
  float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear*distance + pointLights[0].quadratic*distance*distance);

  light += ambientLight * attenuation + diffuseLight*attenuation + specularLight * attenuation;

  fragColor = vec4(diffuseColor.xyz*light, 1.0);
}
