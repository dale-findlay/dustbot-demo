--vertex--
#version 440
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;

out vec4 fPosition;
out vec4 fNormal;
out vec2 fTextureCoord;

void main()
{
    fPosition = model * position;
    //fNormal = normalize(normalMat * normal);
    fNormal = normal;
    fTextureCoord = texCoord;

    gl_Position = projection * view * model * position;
}

--fragment--
#version 440
#define MAX_LIGHT_COUNT 100

#define PI 3.14159265

uniform vec3 cameraPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;

in vec4 fPosition;
in vec4 fNormal;
in vec2 fTextureCoord;

uniform sampler2D diffuseTexture;

struct Light{
    int lightType;
    vec4 lightPos;
    vec4 lightDirection;
};

uniform Light lights[MAX_LIGHT_COUNT];

out vec4 FragColor;

vec4 DirectionalLight(vec4 lightDirection, vec4 viewDirection)
{
    float diff = max(dot(fNormal, normalize(lightDirection)), 0.0) + 0.1;

    return vec4(diff,diff,diff,1.0) * texture(diffuseTexture, fTextureCoord);
}

vec4 PointLight(vec4 lightDirection, vec4 viewDirection)
{
    float diff = dot(fNormal, lightDirection) * 0.8;
    float distance = length(lightDirection);  

    float attenuation = 10.0f + (0.0001f * distance) + (0.02f * distance * distance);

    float attenuatedDiff = (diff) / attenuation;

    return attenuatedDiff * texture(diffuseTexture, fTextureCoord);    
}

void main()
{
    vec4 viewDirection = normalize(vec4(cameraPosition, 1.0f) - fPosition);
    vec4 accum = vec4(0.0f);

    for(int i = 0; i < lights.length(); ++i)
    {
        if(lights[i].lightType == 1)
        {
            accum += DirectionalLight(lights[i].lightDirection, viewDirection);
        }
        else if(lights[i].lightType == 2)
        {
            vec4 pointLightDir = lights[i].lightPos - fPosition;

            accum += PointLight(pointLightDir, viewDirection) / 2;
        }
    }
    


    FragColor = vec4(accum);
}

