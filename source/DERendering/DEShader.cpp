#include "../../include/DECore.hpp"
#include "../../include/DEOpenGL4.hpp"
#include "../../include/DERendering.hpp"
#include <fstream>

void DvigEngine::ShaderComponent::Init(const char* vertexShaderPathOnDrive, const char* fragmentShaderPathOnDrive)
{
    DV_ASSERT_PTR(vertexShaderPathOnDrive)
    DV_ASSERT_PTR(fragmentShaderPathOnDrive)

    DvigEngine::Engine* engine = this->GetEngine();

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
    DvigEngine::MemoryObject* vertexShaderTempMemoryObject = DvigEngine::Engine::Allocate( 0, vertexShaderDataByteWidth );
    vertexShaderData = vertexShaderTempMemoryObject->Unwrap<deuchar*>();
    DvigEngine::MemoryObject* fragmentTempMemoryObject = DvigEngine::Engine::Allocate( 0, fragmentShaderDataByteWidth );
    fragmentShaderData = fragmentTempMemoryObject->Unwrap<deuchar*>();

    // Read files
    vertexFileStream.read((char*)vertexShaderData, vertexShaderDataByteWidth);
    fragmentFileStream.read((char*)fragmentShaderData, fragmentShaderDataByteWidth);

    // Create OpenGL program
    this->m_GLProgram = DvigEngine::GL4::CreateProgram();
    const DvigEngine::deuint32 vertexShader = DvigEngine::GL4::CreateShader( GL_VERTEX_SHADER );
    const DvigEngine::deuint32 fragmentShader = DvigEngine::GL4::CreateShader( GL_FRAGMENT_SHADER );
    DvigEngine::GL4::ShaderSource( vertexShader, 1, (const deuchar**)&vertexShaderData, &vertexShaderDataByteWidth );
    DvigEngine::GL4::ShaderSource( fragmentShader, 1, (const deuchar**)&fragmentShaderData, &fragmentShaderDataByteWidth );
    DvigEngine::GL4::CompileShader( vertexShader );
    DvigEngine::GL4::CompileShader( fragmentShader );
    DvigEngine::GL4::AttachShader( this->m_GLProgram, vertexShader );
    DvigEngine::GL4::AttachShader( this->m_GLProgram, fragmentShader );
    DvigEngine::GL4::LinkProgram( this->m_GLProgram );

    deisize maxByteWidth = 1024;
	deuchar infoLog[1024] = {};
	DvigEngine::GL4::GetShaderInfoLog(vertexShader, maxByteWidth, &maxByteWidth, &infoLog[0]);
    std::cout << &infoLog[0] << std::endl;
    DvigEngine::GL4::GetShaderInfoLog(fragmentShader, maxByteWidth, &maxByteWidth, &infoLog[0]);
    std::cout << &infoLog[0] << std::endl;
    deint32 isLinked = 0;
    DvigEngine::GL4::GetProgramiv(this->m_GLProgram, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) { std::cout << "Error linking shader program!" << std::endl; }

    DvigEngine::GL4::DetachShader( this->m_GLProgram, vertexShader );
    DvigEngine::GL4::DetachShader( this->m_GLProgram, fragmentShader );

    // Delete temporary memory objects
    engine->Delete( vertexShaderTempMemoryObject );
    engine->Delete( fragmentTempMemoryObject );
}

void DvigEngine::ShaderComponent::Free()
{
    DvigEngine::GL4::DeleteProgram( this->m_GLProgram );
    this->GetEngine()->Delete( this->GetMemoryObject() );
}