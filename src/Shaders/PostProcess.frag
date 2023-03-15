#version 460 core

uniform sampler2D renderTexture;

in vec3 fragPos;
out vec4 FragColor;

void main(){
	vec2 uv = (fragPos.xy + 1.0) / 2.0;
	
	FragColor = textureLod(renderTexture, uv, 0.0);
}


