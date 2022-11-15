#version 420

layout (location = 0) in vec3 vsIn_Position;
layout (location = 0) out vec3 vsOut_Position;

void main()
{
    vsOut_Position = vsIn_Position;
    
    gl_Position = vec4(vsIn_Position.xyz, 1.0);
}