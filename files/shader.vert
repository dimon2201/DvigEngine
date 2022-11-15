#version 420

layout (location = 0) in vec3 vsIn_Position;
layout (location = 0) out vec3 vsOut_Position;

struct UInstanceData {
    vec3 m_Position;
};

layout (std140, binding = 0) uniform UBuffer {
    UInstanceData instances[4];
} u_Buffer;

void main()
{
    vsOut_Position = vsIn_Position;
    
    int instanceIndex = gl_InstanceID;
    vec3 position = vsIn_Position + u_Buffer.instances[instanceIndex].m_Position;
    
    gl_Position = vec4(position, 1.0);
}