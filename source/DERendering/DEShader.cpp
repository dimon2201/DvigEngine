#include "../../include/dvigengine/DED3D11.hpp"
#include "../../include/dvigengine/DERendering.hpp"
#include <fstream>

void DvigEngine::ShaderComponent::Init(const char* shaderPathOnDrive)
{
    Engine* engine = Engine::GetClassInstance();

    uint8* vertexShaderData;
    uint8* pixelShaderData;
    sint32 vertexShaderDataByteWidth;
    sint32 pixelShaderDataByteWidth;

    // Open files
    std::ifstream vertexFileStream( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine2\\file\\shaders\\renderpass_vertex.ext", std::ios::binary );
    std::ifstream pixelFileStream( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine2\\file\\shaders\\renderpass_pixel.ext", std::ios::binary );

    vertexFileStream.seekg(0, std::ios::end);
    vertexShaderDataByteWidth = (sint32)vertexFileStream.tellg();
    vertexFileStream.seekg(0, std::ios::beg);

    pixelFileStream.seekg(0, std::ios::end);
    pixelShaderDataByteWidth = (sint32)pixelFileStream.tellg();
    pixelFileStream.seekg(0, std::ios::beg);

    // Temporary memory objects
    MemoryObject* vertexShaderTempMemoryObject = engine->MemoryAllocate( 0, vertexShaderDataByteWidth );
    vertexShaderData = (uint8*)vertexShaderTempMemoryObject->GetAddress();
    DvigEngine::MemoryObject* pixelShaderTempMemoryObject = engine->MemoryAllocate( 0, pixelShaderDataByteWidth );
    pixelShaderData = (uint8*)pixelShaderTempMemoryObject->GetAddress();

    // Read files
    vertexFileStream.read((char*)vertexShaderData, vertexShaderDataByteWidth);
    pixelFileStream.read((char*)pixelShaderData, pixelShaderDataByteWidth);

    // Close files
    vertexFileStream.close();
    pixelFileStream.close();

    // Load bytecode
    HRESULT result;

    // Create instances
    result = D3D11::m_Device->CreateVertexShader( vertexShaderData, vertexShaderDataByteWidth, nullptr, &this->m_VertexShader );
    if (FAILED( result )) {
        std::cout << "Error creating Vertex Shader!" << std::endl;
    }

    result = D3D11::m_Device->CreatePixelShader( pixelShaderData, pixelShaderDataByteWidth, nullptr, &this->m_PixelShader );
    if (FAILED( result )) {
        std::cout << "Error creating Pixel Shader!" << std::endl;
    }

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
    if (FAILED( result )) {
        std::cout << "Error creating Input Layout!" << std::endl;
    }

    // Delete temp memory objects
    engine->MemoryDelete( vertexShaderTempMemoryObject );
    engine->MemoryDelete( pixelShaderTempMemoryObject );
}

void DvigEngine::ShaderComponent::Free()
{
    this->m_VertexShader->Release();
    this->m_PixelShader->Release();
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}