--vertex--
#version 440
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fTextureCoord;

void main()
{
    fTextureCoord = position.xyz;

    gl_Position = projection * view * model * position;
}

--fragment--
#version 440
in vec3 fTextureCoord;

uniform samplerCube skyboxTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(skyboxTexture, fTextureCoord);
}

