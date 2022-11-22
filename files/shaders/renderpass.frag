#version 420

layout (location = 0) in vec3 vsOut_Position;
layout (location = 1) in vec3 vsOut_Normal;
layout (location = 0) out vec4 fsOut_FragColor;

void main()
{
    vec3 color = vec3(0.5) + (vec3(0.5) * vsOut_Normal);

    fsOut_FragColor = vec4(color, 1.0);
}