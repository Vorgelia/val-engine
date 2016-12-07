#version 330

in vec2 out_uv;

uniform sampler2D tex0;
uniform vec4 tex0_params;
uniform vec4 ve_color=vec4(1,1,1,1);
uniform vec4 ve_tintColor=vec4(1,1,1,1);

out vec4 OUT0;

vec2 transformUV(vec2 uv,vec4 params){
	return vec2(uv.x*params.z,1-(1-uv.y)*params.w)+vec2(params.x,-params.y);
}

void main(){
	OUT0=ve_tintColor*ve_color;
	OUT0.a=texture(tex0,transformUV(out_uv,tex0_params)).r;
}

