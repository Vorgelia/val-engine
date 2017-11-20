#version 330

in vec2 out_uv;
in vec3 out_normal;
in float out_depth;

uniform sampler2D tex0;
uniform vec4 tex0_params=vec4(0,0,1,1);

uniform vec4 ve_time;
uniform vec4 ve_screen;
uniform vec4 ve_color=vec4(1,1,1,1);

out vec4 OUT0;
out vec4 OUT1;

vec2 transformUV(vec2 uv,vec4 params){
	return vec2(uv.x*params.z,1-(1-uv.y)*params.w)+vec2(params.x,-params.y);
}

void main(){
	vec4 tex = texture(tex0, transformUV(out_uv, tex0_params));
	if(tex.a<0.1)
		discard;
	
	OUT0=ve_color * tex;
	OUT1=vec4(out_depth,0,1,1);
}