struct VSOut {
	float4 position : SV_Position;
	float2 uv : TexCoord;
};

static const float pi = 3.14159;
const float time;

float sinPragma(float pos) {
	return 0.5f * sin(pos) + 0.5f;
}

float4 main(VSOut input) : SV_Target {
	float2 p = input.uv;
	float2 q = p - float2(0.5 + 0.1 * sin(time * pi), 0.5);

	float3 col = float3(p, 1.0);

	float z = 0.2 + 0.1 * cos(time * pi * 2.0) + 0.01 * sin(time * pi * 8.0) * 2 * cos(atan(q) * (80.0 * sinPragma(pi * time) + 40.0));
	col *= smoothstep(z, z + 0.01, length(q));

	return float4(col, 1.0);
}