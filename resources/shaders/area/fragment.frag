#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D areaData;
uniform sampler2D atlas;

void main()
{
    vec4 texColor = texture(areaData, TexCoords);

    if (texColor.a < 0.5)
        discard;
        
    FragColor = texColor;
}