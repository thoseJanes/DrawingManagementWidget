#version 330 core
layout (location = 0) in vec3 vertices;//Îª×óÏÂ½ÇÎ»ÖÃ¡£
out vec3 fColor;

uniform vec3 myColor;
uniform mat4 model;


void main() {
	gl_Position = model * vec4(vertices,1.0);
	fColor = myColor;
}