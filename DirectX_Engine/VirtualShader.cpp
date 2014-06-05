#include "VirtualShader.h"

VirtualShader::VirtualShader() :
	vertexShader(nullptr),
	pixelShader(nullptr),
	layout(nullptr),
	matrixBuffer(nullptr)
{
}

VirtualShader::~VirtualShader()
{
}

void
VirtualShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("shader-error.txt");
	for(i=0; i<bufferSize; i++)
		fout << compileErrors[i];
	fout.close();
	errorMessage->Release();
	errorMessage = 0;
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}