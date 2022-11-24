#version 420

layout (location = 0) in vec3 vsOut_Position;
layout (location = 1) in vec3 vsOut_Normal;
layout (location = 0) out vec4 fsOut_FragColor;

uniform sampler2DArray u_TextureAtlas;

void main()
{
    vec3 texCoordTest = vec3( 0.0, 0.0, 0.0 );
    // vec3 color = vec3(0.5) + (vec3(0.5) * vsOut_Normal);

    vec4 color = texture(u_TextureAtlas, texCoordTest);

    fsOut_FragColor = vec4(color, 1.0);
}