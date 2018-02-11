#version 420

in vec2 vertex;
in vec2 uv;

out vec2 out_uv;

uniform mat4 ve_matrix_mvp;

void main(){
	out_uv=vec2(uv.x,uv.y);
	gl_Position=ve_matrix_mvp*vec4(vertex.xy,0,1);
}
