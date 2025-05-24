#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 tcolor;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords) * vec4(tcolor, 1);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}