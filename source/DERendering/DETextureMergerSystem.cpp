#include "../../include/DEOpenGL4.hpp"
#include "../../include/DERendering.hpp"

DvigEngine::deusize DvigEngine::TextureMergerSystem::m_AtlasWidth = 1024;
DvigEngine::deusize DvigEngine::TextureMergerSystem::m_AtlasHeight = 1024;
DvigEngine::deusize DvigEngine::TextureMergerSystem::m_AtlasDepth = 64;
DvigEngine::deuint32 DvigEngine::TextureMergerSystem::m_Atlas = DV_NULL;
DvigEngine::FixedSet* DvigEngine::TextureMergerSystem::m_Textures = nullptr;

void DvigEngine::TextureMergerSystem::Init(deusize atlasWidth, deusize atlasHeight, deusize atlasDepth)
{
    if (TextureMergerSystem::m_Atlas == DV_NULL)
    {
        // Initialize atlas size
        if (atlasWidth != DV_NULL && atlasHeight != DV_NULL && atlasDepth != DV_NULL)
        {
            TextureMergerSystem::m_AtlasWidth = atlasWidth;
            TextureMergerSystem::m_AtlasHeight = atlasHeight;
            TextureMergerSystem::m_AtlasDepth = atlasDepth;
        }

        // Create array texture atlas
        GL4::GenTextures( 1, &TextureMergerSystem::m_Atlas );
        GL4::BindTexture( GL_TEXTURE_2D_ARRAY, TextureMergerSystem::m_Atlas );
        GL4::TexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, TextureMergerSystem::m_AtlasWidth, TextureMergerSystem::m_AtlasHeight, TextureMergerSystem::m_AtlasDepth, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );
        GL4::TexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        GL4::TexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        GL4::BindTexture( GL_TEXTURE_2D_ARRAY, 0 );

        // Create texture set
        Engine* engine = Engine::GetClassInstance();
        engine->Create<FixedSet>( &TextureMergerSystem::m_Textures, "_TextureAtlasTextures" );
        TextureMergerSystem::m_Textures->Init( 0, 2, sizeof(TextureMergerTexture) );
    }
}

void DvigEngine::TextureMergerSystem::Free()
{
    GL4::DeleteTextures( 1, &TextureMergerSystem::m_Atlas );
    
    Engine* engine = Engine::GetClassInstance();
    engine->Delete( TextureMergerSystem::m_Textures->GetMemoryObject() );
}

DvigEngine::deint32 DvigEngine::TextureMergerSystem::AddTexture(const deusize width, const deusize height, const void* pixels)
{
    if (TextureMergerSystem::m_Atlas == DV_NULL) { return DV_NULL; }

    // Insert texture to atlas
    const deusize atlasWidth = TextureMergerSystem::m_AtlasWidth;
    const deusize atlasHeight = TextureMergerSystem::m_AtlasHeight;
    const deusize atlasDepth = TextureMergerSystem::m_AtlasDepth;
    if (width > atlasWidth || height > atlasHeight || pixels == nullptr) { return DV_NULL; }

    // Search for free space
    deint32 textureAtlasIndex = DV_NULL;
    const deusize textureWidth = width;
    const deusize textureHeight = height;
    for (deint32 zz = 0; zz < atlasDepth; ++zz)
    {
        for (deint32 yy = 0; yy < atlasHeight - height; ++yy)
        {
            for (deint32 xx = 0; xx < atlasWidth - width; ++xx)
            {
                // Run through all textures
                debool isIntersecting = DV_FALSE;
                for (deint32 idx = 0; idx < TextureMergerSystem::m_Textures->GetCapacity(); ++idx)
                {
                    TextureMergerTexture* getTexture = TextureMergerSystem::m_Textures->Find<TextureMergerTexture*>(idx);

                    // Check if intersects
                    if ( (getTexture->m_X <= (xx + textureWidth)) &&
                         ((getTexture->m_X + getTexture->m_Width) >= xx) &&
                         (getTexture->m_Y <= (yy + textureHeight)) &&
                         ((getTexture->m_Y + getTexture->m_Height) >= yy) &&
                         (getTexture->m_Z == zz) )
                    {
                        isIntersecting = DV_TRUE;
                    }
                }

                if (isIntersecting == DV_FALSE)
                {
                    // We found free block for texture
                    // Insert it to texture set
                    TextureMergerTexture texture;
                    texture.m_X = xx;
                    texture.m_Y = yy;
                    texture.m_Z = zz;
                    texture.m_Width = textureWidth;
                    texture.m_Height = textureHeight;
                    textureAtlasIndex = TextureMergerSystem::m_Textures->Insert( &texture );
                    
                    // Populate array texture atlas
                    GL4::BindTexture( GL_TEXTURE_2D_ARRAY, TextureMergerSystem::m_Atlas );
                    GL4::TexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, texture.m_X, texture.m_Y, texture.m_Z, texture.m_Width, texture.m_Height, atlasDepth, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                    GL4::BindTexture( GL_TEXTURE_2D_ARRAY, 0 );

                    // Exit loops
                    goto exit_loop;
                }
            }
        }
    }

    exit_loop:
    {
        return textureAtlasIndex;
    }
}

void DvigEngine::TextureMergerSystem::RemoveTexture(const deint32 index)
{
    if (index == DV_NULL || index >= TextureMergerSystem::m_Textures->GetCapacity()) { return; }

    TextureMergerSystem::m_Textures->Remove( index );
}