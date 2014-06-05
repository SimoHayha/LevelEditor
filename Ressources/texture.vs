cbuffer	MatrixBuffer
{
	matrix	worldMatrix;
	matrix	viewMatrix;
	matrix	projectionMatrix;
}

cbuffer	cameraBuffer
{
	float4	camPos;
};

struct	VertexInputType
{
	float4	position : POSITION;
	float4	normal : NORMAL;
	float2	tex : TEXCOORD0;
};

struct	PixelInputType
{
	float4	position : SV_POSITION;
	float2	tex : TEXCOORD0;
	float4	normal : NORMAL;
	float4	worldPos : TEXCOORD1;
	float3	viewDirection : TEXCOORD2;
};

PixelInputType	TextureVertexShader(VertexInputType input)
{
	PixelInputType	output;
	float4			worldPosition;

	input.position.w = 1.0f;

	worldPosition = mul(input.position, worldMatrix);
	output.worldPos = worldPosition;
	output.position = worldPosition;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);


	float3 normal = mul(input.normal.xyz, worldMatrix);
	normal = normalize(normal);
	output.normal.xyz = normal;
	output.tex = input.tex;

	output.viewDirection = camPos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	return output;
}