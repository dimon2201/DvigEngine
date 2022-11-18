#version 420

layout (location = 0) in vec3 vsIn_Position;
layout (location = 1) in vec3 vsIn_Normal;
layout (location = 0) out vec3 vsOut_Position;
layout (location = 1) out vec3 vsOut_Normal;

struct UViewer {
    mat4 m_WorldSpaceMatrix;
    mat4 m_ViewSpaceMatrix;
    mat4 m_ProjectionSpaceMatrix;
};

struct UInstanceData {
    mat4 m_WorldSpaceMatrix;
};

layout (std140, binding = 0) uniform UBuffer {
    UViewer m_Viewer;
    UInstanceData m_Instances[256];
} u_Buffer;

void main()
{
    vsOut_Position = vsIn_Position;
    vsOut_Normal = vsIn_Normal;
    
    int instanceIndex = gl_InstanceID;
    vec3 scale = vec3(0.7);
    vec3 position = scale * vsIn_Position;
    // if (gl_VertexID == 0) { position = vec3(-1.0,-1.0,0.0); }
    // if (gl_VertexID == 1) { position = vec3(0.0,1.0,0.0); }
    // if (gl_VertexID == 2) { position = vec3(1.0,-1.0,0.0); }
    
    gl_Position = u_Buffer.m_Viewer.m_ViewSpaceMatrix * vec4(position, 1.0);
}