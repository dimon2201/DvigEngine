#version 420

layout (location = 0) in vec3 vsOut_Position;
layout (location = 0) out vec4 fsOut_FragColor;

struct UPostProcessor {
    sampler2D m_ColorRenderTarget;
    sampler2D m_DepthRenderTarget;
};

uniform UPostProcessor u_PostProcessor;

void main()
{
    vec2 texcoord = vec2(vsOut_Position.xy);
    vec4 color = texture(u_PostProcessor.m_ColorRenderTarget, texcoord);

    fsOut_FragColor = vec4(color.xyz, 1.0);
}