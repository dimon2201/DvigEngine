#version 420
#extension GL_ARB_shader_storage_buffer_object: require

layout (location = 0) in vec3 vsOut_Position;
layout (location = 1) in vec2 vsOut_Texcoord;
layout (location = 2) in vec3 vsOut_Normal;
layout (location = 3) in flat uint vsOut_InstanceIndex;
layout (location = 0) out vec4 fsOut_FragColor;

struct UConstants {
    uvec3 m_TextureAtlasDimensions;
};

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

layout (std140, binding = 0) buffer UBuffer {
    UConstants m_Constants;
    UViewer m_Viewer;
    UInstanceData m_Instances[256];
} u_Buffer;

void main()
{
    // vec3 texCoordTest = vec3( 0.0, 0.0, 0.0 );
    // vec3 color = vec3(0.5) + (vec3(0.5) * vsOut_Normal);

    uint instanceIndex = vsOut_InstanceIndex;

    uvec4 instDiffuseTextureInfo = u_Buffer.m_Instances[instanceIndex].m_DiffuseTextureInfo;
    float diffuseTextureWidth = float(instDiffuseTextureInfo.y >> 16);
    float diffuseTextureHeight = float(instDiffuseTextureInfo.z & 65535);

    vec2 textureTexelSize = vec2(diffuseTextureWidth, diffuseTextureHeight) * vec2(1.0 / 1024.0);
    vec4 color = texture2DArray(u_TextureAtlas, vec3(vsOut_Texcoord * textureTexelSize, 0.0));

    fsOut_FragColor = vec4(color.xyz, 1.0);
}