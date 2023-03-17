#version 460 core

vec3 ACESFilm(vec3 x);

uniform sampler2D finalRenderTexture;

in vec3 fragPos;
out vec4 FragColor;


void main(){
	vec2 uv = (fragPos.xy + 1.0) / 2.0;
	
    vec3 color = textureLod(finalRenderTexture, uv, 0.0).rgb;

	FragColor = vec4(ACESFilm(color), 1.0);
}


vec3 ACESFilm(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}