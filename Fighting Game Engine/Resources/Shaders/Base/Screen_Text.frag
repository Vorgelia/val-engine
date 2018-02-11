#version 420
#pragma include Shaders/Base/Data/SurfaceInclude.veinc

in vec2 out_uv;

uniform sampler2D tex0;
uniform vec4 tex0_params;
uniform vec4 ve_color=vec4(1,1,1,1);
uniform vec4 ve_tintColor=vec4(1,1,1,1);

out vec4 OUT0;

void main(){
	OUT0=ve_tintColor*ve_color;
	OUT0.a=texture(tex0,TransformUV(out_uv,tex0_params)).r;
}

