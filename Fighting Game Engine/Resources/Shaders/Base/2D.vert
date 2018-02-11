#version 420

in vec2 vertex;
in vec2 uv;
in vec3 normal;

out vec2 out_uv;
out vec3 out_normal;
out float out_depth;

uniform mat4 ve_matrix_mvp;
uniform float ve_depth;

void main(){
	out_uv = uv;
	out_normal = normal;
	gl_Position = ve_matrix_mvp * vec4(vertex.xy, 0, 1);
	out_depth = (1 + gl_Position.z) * 0.5;
}