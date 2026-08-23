#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 inputColor;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    vec4 color = vec4(inputColor, 1);
    FragColor = color;
}