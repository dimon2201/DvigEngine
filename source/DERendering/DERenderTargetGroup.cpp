#include "../../include/DEOpenGL4.hpp"
#include "../../include/DERendering.hpp"
#include "../../include/DEThirdPartyMath.hpp"

void DvigEngine::RenderTargetGroup::Init(glm::uvec2& size)
{
    GL4::GenTextures( 2, &this->m_RenderTargets[0] );
    GL4::BindTexture( GL_TEXTURE_2D, this->m_RenderTargets[0] ); // color
    GL4::TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, (deisize)size.x, (deisize)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );
    GL4::TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    GL4::TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    GL4::BindTexture( GL_TEXTURE_2D, this->m_RenderTargets[1] ); // depth
    GL4::TexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (deisize)size.x, (deisize)size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr );
    GL4::TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    GL4::TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    GL4::BindTexture( GL_TEXTURE_2D, 0 );
    GL4::GenFramebuffers( 1, &this->m_Framebuffer );
    GL4::BindFramebuffer( GL_FRAMEBUFFER, this->m_Framebuffer );
    GL4::FramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_RenderTargets[0], 0 );
    GL4::FramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->m_RenderTargets[1], 0 );
    GL4::BindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void DvigEngine::RenderTargetGroup::Free()
{
    GL4::DeleteTextures( 2, &this->m_RenderTargets[0] );
    GL4::DeleteFramebuffers( 1, &this->m_Framebuffer );
    this->GetEngine()->Delete( this->GetMemoryObject() );
}