#version 420

layout (location = 0) in vec3 a_Position;

void main()
{
    vec3 position = vec3(0.0);
    if (gl_VertexID == 0) {
        position = vec3(-1.0, -1.0, -1.0);
    } else if (gl_VertexID == 1) {
        position = vec3(0.0, 1.0, -1.0);
    } else if (gl_VertexID == 2) {
        position = vec3(1.0, -1.0, -1.0);
    }
    
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}