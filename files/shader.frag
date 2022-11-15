#version 420

layout (location = 0) in vec3 vsIn_Position;
layout (location = 0) out vec4 fsOut_FragColor;

void main()
{
    vec3 color = vec3(0.5) + (vec3(0.5) * vsIn_Position);

    fsOut_FragColor = vec4(color, 1.0);
}