struct VSIn {
    float3 positionLocalSpace 	: DE_POSITION;
	float2 texcoord				: DE_TEXCOORD;
	float3 normalLocalSpace		: DE_NORMAL;
	uint vertexIndex			: SV_VertexID;
	uint instanceIndex			: SV_InstanceID;
};

struct VSOut {
    float4 positionClipSpace 	: SV_POSITION;
	//float2 texcoord				: DE_TEXCOORD;
	float3 normalLocalSpace		: DE_NORMAL;
};

struct Instance {
	float4x4 m_WorldSpaceMatrix;
};

cbuffer BufferViewer : register(b0) {
	float4x4 m_ViewSpaceMatrix;
	float4x4 m_ProjectionSpaceMatrix;
};
StructuredBuffer<Instance> u_BufferInstance : register( t0 );

VSOut vs_main(VSIn vsIn)
{
	float4 positionWorldSpace = mul(u_BufferInstance[vsIn.instanceIndex].m_WorldSpaceMatrix, float4(vsIn.positionLocalSpace, 1.0));

	VSOut vsOut;
	vsOut.positionClipSpace = mul(m_ProjectionSpaceMatrix, mul(m_ViewSpaceMatrix, positionWorldSpace));
	// vsOut.positionClipSpace.x += u_Buffer[0].value;
	//vsOut.texcoord = vsIn.texcoord;
	vsOut.normalLocalSpace = vsIn.normalLocalSpace;

	// if (vsIn.vertexIndex == 0) { vsOut.positionClipSpace = float4(-1.0, -1.0, 0.5, 1.0); }
	// if (vsIn.vertexIndex == 1) { vsOut.positionClipSpace = float4(0.0, 1.0, 0.5, 1.0); }
	// if (vsIn.vertexIndex == 2) { vsOut.positionClipSpace = float4(1.0, -1.0, 0.5, 1.0); }
	
	return vsOut;
}

float4 ps_main(VSOut fsIn) : SV_TARGET
{
	return float4( fsIn.normalLocalSpace, 1.0 );
}