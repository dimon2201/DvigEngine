#include "../../Include/DvigEngine/D3D11.hpp"
#include "../../Include/DvigEngine/Rendering.hpp"
#include <fstream>

void DvigEngine::ShaderComponent::Init()
{
}

void DvigEngine::ShaderComponent::Free()
{
    this->m_VertexShader->Release();
    this->m_PixelShader->Release();
}

void DvigEngine::ShaderComponent::SetVertexShader(const char* shaderPathOnDrive)
{
    Engine* engine = Engine::GetClassInstance();

    uint8* vertexShaderData;
    sint32 vertexShaderDataByteWidth;

    // Open files
    std::ifstream vertexFileStream( shaderPathOnDrive, std::ios::binary );

    vertexFileStream.seekg(0, std::ios::end);
    vertexShaderDataByteWidth = (sint32)vertexFileStream.tellg();
    vertexFileStream.seekg(0, std::ios::beg);

    // Temporary memory objects
    MemoryObject* vertexShaderTempMemoryObject = engine->MemoryAllocate( 0, vertexShaderDataByteWidth );
    vertexShaderData = (uint8*)vertexShaderTempMemoryObject->GetAddress();

    // Read files
    vertexFileStream.read((char*)vertexShaderData, vertexShaderDataByteWidth);

    // Close files
    vertexFileStream.close();

    // Load bytecode
    HRESULT result;

    // Create instances
    result = D3D11::m_Device->CreateVertexShader( vertexShaderData, vertexShaderDataByteWidth, nullptr, &this->m_VertexShader );

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[3] = {};
    inputElementDesc[0].SemanticName = "DE_POSITION";
    inputElementDesc[0].SemanticIndex = 0;
    inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[0].InputSlot = 0;
    inputElementDesc[0].AlignedByteOffset = 0;
    inputElementDesc[0].InstanceDataStepRate = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc[1].SemanticName = "DE_TEXCOORD";
    inputElementDesc[1].SemanticIndex = 0;
    inputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDesc[1].InputSlot = 0;
    inputElementDesc[1].AlignedByteOffset = 12;
    inputElementDesc[1].InstanceDataStepRate = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc[2].SemanticName = "DE_NORMAL";
    inputElementDesc[2].SemanticIndex = 0;
    inputElementDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[2].InputSlot = 0;
    inputElementDesc[2].AlignedByteOffset = 20;
    inputElementDesc[2].InstanceDataStepRate = D3D11_INPUT_PER_VERTEX_DATA;

    result = D3D11::m_Device->CreateInputLayout( &inputElementDesc[0], 3, vertexShaderData, vertexShaderDataByteWidth, &this->m_InputLayout );

    // Delete temp memory objects
    engine->MemoryDelete( vertexShaderTempMemoryObject );
}

void DvigEngine::ShaderComponent::SetPixelShader(const char* shaderPathOnDrive)
{
    Engine* engine = Engine::GetClassInstance();

    uint8* pixelShaderData;
    sint32 pixelShaderDataByteWidth;

    // Open files
    std::ifstream pixelFileStream( shaderPathOnDrive, std::ios::binary );

    pixelFileStream.seekg(0, std::ios::end);
    pixelShaderDataByteWidth = (sint32)pixelFileStream.tellg();
    pixelFileStream.seekg(0, std::ios::beg);

    // Temporary memory objects
    DvigEngine::MemoryObject* pixelShaderTempMemoryObject = engine->MemoryAllocate( 0, pixelShaderDataByteWidth );
    pixelShaderData = (uint8*)pixelShaderTempMemoryObject->GetAddress();

    // Read files
    pixelFileStream.read((char*)pixelShaderData, pixelShaderDataByteWidth);

    // Close files
    pixelFileStream.close();

    // Load bytecode
    HRESULT result;

    // Create instances
    result = D3D11::m_Device->CreatePixelShader( pixelShaderData, pixelShaderDataByteWidth, nullptr, &this->m_PixelShader );

    // Delete temp memory objects
    engine->MemoryDelete( pixelShaderTempMemoryObject );
}