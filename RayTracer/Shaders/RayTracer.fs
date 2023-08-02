#version 430
out vec4 FragColor;  
uniform sampler2D screenImageData;
in vec2 UV;

void main()
{
    FragColor = texture(screenImageData, UV);

}