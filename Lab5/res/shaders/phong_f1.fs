//this var comes from the vertex shader
//they are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_wPos;
varying vec3 v_wNormal;
varying vec2 v_coord;

//here create uniforms for all the data we need here
uniform vec3 u_camera_pos;
uniform vec3 u_light_pos;
uniform sampler2D u_texture;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform float u_alpha;

uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

void main()
{
	//here we set up the normal as a color to see them as a debug
	vec3 color = v_wNormal;

	//here write the computations for PHONG.
	//for GOURAUD you dont need to do anything here, just pass the color from the vertex shader
	vec4 texcol = texture2D( u_texture, v_coord );

	vec3 L = normalize(u_light_pos - v_wPos);

	vec3 R = -reflect(L, v_wNormal);

	vec3 V = normalize(u_camera_pos - v_wPos);

	//u_Ka = texcol.xyz;
	//u_Kd = texcol.xyz;
	//u_Ks = texcol.xyz;

	vec3 La = texcol.xyz * u_Ia;
	vec3 Ld = texcol.xyz * u_Id * max(dot(L,v_wNormal),0.0);
	vec3 Ls = texcol.xyz * u_Is * pow(max(dot(R,V),0.0),u_alpha);

	color = La+Ld+Ls;

	//set the ouput color por the pixel
	gl_FragColor = vec4( color, 1.0 ) * 1.0;
	//gl_FragColor = color1;
}
