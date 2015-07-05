struct VSOut {
	float4 position : SV_Position;
	float2 uv : TexCoord;
};

VSOut main(uint vertexID : SV_VertexID) {
	VSOut result;

	result.uv = float2((vertexID << 1) & 2, vertexID & 2);
	result.position = float4(result.uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);

	return result;
}