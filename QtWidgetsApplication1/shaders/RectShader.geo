#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 5) out;

uniform vec2 size;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection*view*gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = projection*view*(gl_in[0].gl_Position + vec4(size.x, 0.0, 0.0, 0.0));
    EmitVertex();

    gl_Position = projection*view*(gl_in[0].gl_Position + vec4(size.x, size.y, 0.0, 0.0));
    EmitVertex();

    gl_Position = projection*view*(gl_in[0].gl_Position + vec4(0.0, size.y, 0.0, 0.0));
    EmitVertex();

    gl_Position = projection*view*(gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0));
    EmitVertex();

    EndPrimitive();
}