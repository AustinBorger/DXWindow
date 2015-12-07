struct VSOut {
	float4 position : SV_Position;
	float2 uv : TexCoord;
};

static const float pi = 3.14159f;
const float time;

float sinPragma(float pos) {
	return 0.5f * sin(pos) + 0.5f;
}

float4 main(VSOut input) : SV_Target {
	float2 p = input.uv;
	float2 q = p - float2(0.5f + 0.1f * sin(time * pi), 0.5f);

	float3 col = float3(p, 1.0f);

	float z = 0.2f + 0.1f * cos(time * pi * 2.0) + 0.01f * sin(time * pi * 8.0) * 2.0f * cos(atan(q.x) * (80.0f * sinPragma(pi * time) + 40.0f));
	col *= smoothstep(z, z + 0.01f, length(q));

	return float4(col, 1.0f);
}