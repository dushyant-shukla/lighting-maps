#version 460 core

uniform bool debug;

void main() {
	if(!debug) {
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0);;
	} 
	else {
		gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);	
	}
	
}