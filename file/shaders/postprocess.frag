#version 420

layout (location = 0) in vec3 vsOut_Position;
layout (location = 0) out vec4 fsOut_FragColor;

struct UInputRenderTargets {
    sampler2D m_Color;
    sampler2D m_Depth;
};

uniform UInputRenderTargets u_InputRenderTargets;

void main()
{
    vec2 texcoord = vec2(vsOut_Position.xy);
    vec4 color = texture(u_InputRenderTargets.m_Color, texcoord);

    fsOut_FragColor = vec4(color.xyz, 1.0);
}