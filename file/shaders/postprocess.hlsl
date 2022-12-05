struct VSOut {
    float4 positionClipSpace : SV_POSITION;
};

VSOut vs_main()
{
	VSOut vsOut;
	
	return vsOut;
}

float4 ps_main(VSOut vsIn) : SV_TARGET
{
	return float4( 1.0, 0.0, 1.0, 1.0 );
}