#version 330

in vec2 out_uv;

uniform sampler2D tex0;
uniform sampler2D mainBuf_tex0;

uniform vec4 ve_time;
uniform vec4 ve_screen;

out vec4 OUT0;

void main(){
	OUT0=texture(tex0,out_uv)*0.5+texture(mainBuf_tex0,out_uv)*0.5;
}