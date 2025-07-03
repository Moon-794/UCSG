#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D areaData;
uniform sampler2D atlas;

void main()
{
    vec4 texColor = texture(areaData, TexCoords);
    vec4 color = vec4(1, 0, 0, 1);
    FragColor = color;
}