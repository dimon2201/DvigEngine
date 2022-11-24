#version 420

layout (location = 0) in vec3 vsOut_Position;
layout (location = 1) in vec2 vsOut_Texcoord;
layout (location = 2) in vec3 vsOut_Normal;
layout (location = 0) out vec4 fsOut_FragColor;

struct UViewer {
    mat4 m_WorldSpaceMatrix;
    mat4 m_ViewSpaceMatrix;
    mat4 m_ProjectionSpaceMatrix;
};

struct UInstanceData {
    mat4 m_WorldSpaceMatrix;
    uvec4 m_DiffuseTextureInfo;
};

struct UInputRenderTargets {
    sampler2D m_Color;
    sampler2D m_Depth;
};

uniform UInputRenderTargets u_InputRenderTargets;
uniform mediump sampler2DArray u_TextureAtlas;

void main()
{
    // vec3 texCoordTest = vec3( 0.0, 0.0, 0.0 );
    // vec3 color = vec3(0.5) + (vec3(0.5) * vsOut_Normal);

    vec2 textureTexelSize = 2.0 * vec2(1.0 / 1024.0);
    vec4 color = texture2DArray(u_TextureAtlas, vec3(vsOut_Texcoord * textureTexelSize, 0.0));

    fsOut_FragColor = vec4(color.xyz, 1.0);
}