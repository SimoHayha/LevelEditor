#include "TextureShader.h"
#include "FBX_Material.h"

TextureShader::TextureShader(void) :
	sampleState(nullptr),
	materialBuffer(nullptr),
	cameraBuffer(nullptr),
	lightBuffer(nullptr)
{
}

bool
TextureShader::initialize(ID3D11Device* device, HWND hwnd)
{
	return InitializeShader(device, hwnd, L"texture.vs", L"texture.ps");
}

bool
TextureShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFile, WCHAR* psFile)
{
	ID3D10Blob*					errorMessage = nullptr;
	ID3D10Blob*					vertexShaderBuffer = nullptr;
	ID3D10Blob*					pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC	polygonLayout[3];
	unsigned int				numElements;
	D3D11_BUFFER_DESC			matrixBufferDesc;
	D3D11_BUFFER_DESC			materialBufferDesc;
	D3D11_BUFFER_DESC			cameraBufferDesc;
	D3D11_BUFFER_DESC			lightBufferDesc;
	D3D11_SAMPLER_DESC			samplerDesc;

	if (FAILED(D3DCompileFromFile(vsFile, nullptr, nullptr, "TextureVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFile);
		return false;
	}
	if (FAILED(D3DCompileFromFile(psFile, nullptr, nullptr, "TexturePixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psFile);
		return false;
	}
	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		nullptr, &vertexShader)))
		return false;
	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		nullptr, &pixelShader)))
		return false;
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &layout)))
		return false;
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer)))
		return false;
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(shaderMaterial);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = sizeof(float) * 4;
	if (FAILED(device->CreateBuffer(&materialBufferDesc, nullptr, &materialBuffer)))
		return false;
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	if (FAILED(device->CreateBuffer(&cameraBufferDesc, nullptr, &cameraBuffer)))
		return false;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(Light::shaderLight) * 8;
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	if (FAILED(device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer)))
		return false;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(device->CreateSamplerState(&samplerDesc, &sampleState)))
		return false;
	return true;
}

void
TextureShader::shutdown(void)
{
	ShutdownShader();
}

void
TextureShader::ShutdownShader(void)
{
	
	if (sampleState)
	{
		sampleState->Release();
		sampleState = nullptr;
	}
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}
	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = nullptr;
	}
	if (materialBuffer)
	{
		materialBuffer->Release();
		materialBuffer = nullptr;
	}
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}
	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}

}

bool
TextureShader::render(ID3D11DeviceContext* deviceContext, int indexCount,
						DirectX::XMMATRIX& worldMatrix,
						DirectX::XMMATRIX& viewMatrix,
						DirectX::XMMATRIX& projectionMatrix,
						ID3D11ShaderResourceView* texture, shaderMaterial* mat)
{
	if (SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, mat) == false)
		return false;
	RenderShader(deviceContext, indexCount);
	return true;
}

void
TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(layout);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool
TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext,
								 DirectX::XMMATRIX& worldMatrix,
								 DirectX::XMMATRIX& viewMatrix,
								 DirectX::XMMATRIX& projectionMatrix,
								 ID3D11ShaderResourceView* texture, shaderMaterial* mat)
{
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	MatrixBufferType*			dataPtr;
	shaderMaterial*	materialPtr;

	unsigned int				bufferNumber;

	worldMatrix	= DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	if (FAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	deviceContext->Unmap(matrixBuffer, 0);
	bufferNumber = 0;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	if (FAILED(deviceContext->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;
	materialPtr = (shaderMaterial*)mappedResource.pData;
	if (texture == nullptr)
		mat->ambient.w = 0.0f;
	else
		mat->ambient.w = 1.0f;
	if (mat != nullptr)
	{
		materialPtr->color = mat->color;
		materialPtr->emmisive = mat->emmisive;
		materialPtr->ambient = mat->ambient;
		materialPtr->diffuse = mat->diffuse;
		materialPtr->specular = mat->specular;
		materialPtr->shinness = mat->shinness;
	}
	else
	{
		materialPtr->emmisive = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		materialPtr->ambient = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		materialPtr->diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 0.0f);
		materialPtr->specular = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
		materialPtr->shinness = 0.0f;
	}
	deviceContext->Unmap(materialBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &materialBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
	worldMatrix	= DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	return true;
}

bool
TextureShader::setCameraConstBuffer(ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR&	vector)
{	
	DirectX::XMFLOAT4*			cameraPos;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;

	if (FAILED(deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;
	cameraPos = (DirectX::XMFLOAT4*)mappedResource.pData;
	cameraPos->x = DirectX::XMVectorGetX(vector);
	cameraPos->y = DirectX::XMVectorGetY(vector);
	cameraPos->z = DirectX::XMVectorGetZ(vector);
	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &cameraBuffer);
	return true;
}

bool
TextureShader::setLightConstBuffer(ID3D11DeviceContext* deviceContext, std::vector<Light*>& lightEntities)
{	
	Light::shaderLight*			light;
	Light::shaderLight*			lights;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	size_t						size = lightEntities.size();
	size_t i;

	if (FAILED(deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;
	lights = (Light::shaderLight*)mappedResource.pData;
	ZeroMemory(lights, sizeof(*lights) * 8);
	for (i = 0; i < size ; ++i)
	{
		light = lightEntities[i]->getLight();
		lights[i].position = light->position;
		lights[i].color = light->color;
	}
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &lightBuffer);
	return true;
}
