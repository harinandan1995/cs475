#version 410

in vec4 vPosition;
in vec4 vColor;
out vec4 color;
uniform mat4 uModelViewMatrix;
uniform mat4 orthoMatrix;
uniform mat4 DCSMatrix;
uniform float mode;

void main (void) 
{
	if (mode==5) {
		vec4 v = uModelViewMatrix * vPosition;
		gl_Position = orthoMatrix * DCSMatrix * vec4(v.x/v.w, v.y/v.w, v.z/v.w, 1);
		color = vColor;
	}
	else if (mode == 4){
		vec4 v = uModelViewMatrix * vPosition;
		gl_Position = orthoMatrix * vec4(v.x/v.w, v.y/v.w, v.z/v.w, 1);
		color = vColor;
	}
	else if (mode == 3){
		vec4 v = orthoMatrix * uModelViewMatrix * vPosition;
		gl_Position = vec4(v.x,v.y,v.z,1);
		color = vColor;
	}
	else if (mode == 2){
		gl_Position = orthoMatrix * uModelViewMatrix * vPosition;
		color = vColor;
	}
	else if (mode == 1){
		gl_Position = orthoMatrix * uModelViewMatrix * vPosition;
		color = vColor;
	}
}
