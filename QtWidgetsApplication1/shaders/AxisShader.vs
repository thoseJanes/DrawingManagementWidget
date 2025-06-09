#version 330 core
layout (location = 0) in vec3 vertices;
out vec3 fColor;

uniform vec3 myColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float axisLength;
void main() {
	gl_Position = projection* view* model * vec4(vertices * axisLength,1.0);
	fColor = myColor;
}