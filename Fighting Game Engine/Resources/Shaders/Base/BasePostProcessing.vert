#version 330

in vec2 vertex;
in vec2 uv;

out vec2 out_uv;

uniform vec4 ve_time;
uniform mat4 ve_matrix_mvp;
uniform mat4 ve_matrix_model;
uniform mat4 ve_matrix_projection;

void main(){
	out_uv=uv;
	gl_Position=ve_matrix_mvp*vec4(vertex.xy,0.5,1);
}
