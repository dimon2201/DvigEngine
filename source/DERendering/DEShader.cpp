#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

#include <fstream>

void DvigEngine2::ShaderComponent::Init(const char* vertexShaderPathOnDrive, const char* fragmentShaderPathOnDrive)
{
    DV_ASSERT_PTR(vertexShaderPathOnDrive)
    DV_ASSERT_PTR(fragmentShaderPathOnDrive)

    DvigEngine2::Engine* engine = this->GetEngine();

    deuchar* vertexShaderData;
    deuchar* fragmentShaderData;
    deint32 vertexShaderDataByteWidth;
    deint32 fragmentShaderDataByteWidth;

    // Open files
    std::ifstream vertexFileStream( &vertexShaderPathOnDrive[0], std::ios::binary );
    std::ifstream fragmentFileStream( &fragmentShaderPathOnDrive[0], std::ios::binary );

    vertexFileStream.seekg(0, std::ios::end);
    vertexShaderDataByteWidth = vertexFileStream.tellg();
    vertexFileStream.seekg(0, std::ios::beg);

    fragmentFileStream.seekg(0, std::ios::end);
    fragmentShaderDataByteWidth = fragmentFileStream.tellg();
    fragmentFileStream.seekg(0, std::ios::beg);

    // Temporary memory objects
    DvigEngine2::MemoryObject* vertexShaderTempMemoryObject = DvigEngine2::Engine::Allocate( 0, vertexShaderDataByteWidth );
    vertexShaderData = vertexShaderTempMemoryObject->Unwrap<deuchar*>();
    DvigEngine2::MemoryObject* fragmentTempMemoryObject = DvigEngine2::Engine::Allocate( 0, fragmentShaderDataByteWidth );
    fragmentShaderData = fragmentTempMemoryObject->Unwrap<deuchar*>();

    // Read files
    vertexFileStream.read((char*)vertexShaderData, vertexShaderDataByteWidth);
    fragmentFileStream.read((char*)fragmentShaderData, fragmentShaderDataByteWidth);

    // Create OpenGL program
    this->m_GLProgram = DvigEngine2::GL4::CreateProgram();
    const DvigEngine2::deuint32 vertexShader = DvigEngine2::GL4::CreateShader( GL_VERTEX_SHADER );
    const DvigEngine2::deuint32 fragmentShader = DvigEngine2::GL4::CreateShader( GL_FRAGMENT_SHADER );
    DvigEngine2::GL4::ShaderSource( vertexShader, 1, (const deuchar**)&vertexShaderData, &vertexShaderDataByteWidth );
    DvigEngine2::GL4::ShaderSource( fragmentShader, 1, (const deuchar**)&fragmentShaderData, &fragmentShaderDataByteWidth );
    DvigEngine2::GL4::CompileShader( vertexShader );
    DvigEngine2::GL4::CompileShader( fragmentShader );
    DvigEngine2::GL4::AttachShader( this->m_GLProgram, vertexShader );
    DvigEngine2::GL4::AttachShader( this->m_GLProgram, fragmentShader );
    DvigEngine2::GL4::LinkProgram( this->m_GLProgram );
    DvigEngine2::GL4::DetachShader( this->m_GLProgram, vertexShader );
    DvigEngine2::GL4::DetachShader( this->m_GLProgram, fragmentShader );

    deisize maxByteWidth = 1024;
	deuchar infoLog[1024] = {};
	DvigEngine2::GL4::GetShaderInfoLog(vertexShader, maxByteWidth, &maxByteWidth, &infoLog[0]);
    std::cout << &infoLog[0] << std::endl;
    DvigEngine2::GL4::GetShaderInfoLog(fragmentShader, maxByteWidth, &maxByteWidth, &infoLog[0]);
    std::cout << &infoLog[0] << std::endl;

    // Delete temporary memory objects
    engine->Delete( vertexShaderTempMemoryObject );
    engine->Delete( fragmentTempMemoryObject );
}

void DvigEngine2::ShaderComponent::Free()
{
    DvigEngine2::GL4::DeleteProgram( this->m_GLProgram );
    this->GetEngine()->Delete( this->GetMemoryObject() );
}