#version 420

layout (location = 0) out vec3 vsOut_Position;

struct UPostProcessor {
    sampler2D m_ColorRenderTarget;
    sampler2D m_DepthRenderTarget;
};

uniform UPostProcessor u_PostProcessor;

void main()
{
    vec3 position = vec3(0.0);
    if (gl_VertexID == 0) { position = vec3(-1.0, -1.0, 0.0); }
    if (gl_VertexID == 1) { position = vec3(-1.0, 1.0, 0.0); }
    if (gl_VertexID == 2) { position = vec3(1.0, -1.0, 0.0); }
    if (gl_VertexID == 3) { position = vec3(1.0, 1.0, 0.0); }

    vsOut_Position = (position * 0.5) + 0.5;

    gl_Position = vec4(position, 1.0);
}