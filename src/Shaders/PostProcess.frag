#version 460 core

in vec3 fragPos;
out vec4 FragColor;

void main(){
	vec2 uv = fragPos.xy;
	
	FragColor = vec4(uv, 0, 1);
}


