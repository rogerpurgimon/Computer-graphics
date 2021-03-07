//global variables from the CPU
uniform mat4 model;
uniform mat4 viewprojection;


//vars to pass to the pixel shader
varying vec3 v_wPos;
varying vec3 v_wNormal;

varying vec3 v_Ip;

//here create uniforms for all the data we need here
uniform vec3 u_camera_pos;
uniform vec3 u_light_pos;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform float u_alpha;

uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

void main()
{	
	//convert local coordinate to world coordinates
	vec3 wPos = (model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	//convert local normal to world coordinates
	vec3 wNormal = (model * vec4( gl_Normal.xyz, 0.0)).xyz;

	//pass them to the pixel shader interpolated
	v_wPos = wPos;
	v_wNormal = wNormal;

	//in GOURAUD compute the color here and pass it to the pixel shader
	
	vec3 L = normalize(u_light_pos - v_wPos);

	vec3 R = -reflect(L, v_wNormal);

	vec3 V = normalize(u_camera_pos - v_wPos);

	vec3 La = u_Ka * u_Ia;
	vec3 Ld = u_Kd * max(dot(L,v_wNormal),0.0) * u_Id;
	vec3 Ls = u_Ks * pow(max(dot(R,V),0.0),u_alpha) * u_Is;

	v_Ip = La+Ld+Ls;
	
	//project the vertex by the model view projection 
	gl_Position = viewprojection * vec4(wPos,1.0); //output of the vertex shader
}