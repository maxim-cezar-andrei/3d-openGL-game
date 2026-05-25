#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;


uniform vec3 objectColor;
uniform sampler2D tex;
uniform bool useTexture;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 viewPos;    
uniform vec3 viewDir;

void main()
{
    vec3 culoare = useTexture ? vec3(texture(tex, TexCoord)) : objectColor;

    vec3 norm = normalize(Normal);
    vec3 ambient = 0.25 * lightColor;
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float dist = length(lightPos - FragPos);
    float aten = 1.0 / (1.0 + 0.01 * dist + 0.002 * dist * dist); 
    vec3 diffuse = diff * aten * lightColor;

    vec3 centerLight = (ambient + diffuse) * culoare;
    
    vec3 dirFlash = normalize(viewPos - FragPos);
    float theta = dot(dirFlash, normalize(-viewDir));
    float cutoff = cos(radians(30.0)); // conul de lumina - 30 grade

    vec3 flash = vec3(0.0);
    if (theta > cutoff)
    {
        float flashDiff = max(dot(norm, dirFlash), 0.0);
        float flashDist = length(viewPos - FragPos);
        float flashAten = 1.0 / (1.0 + 0.03 * flashDist + 0.005 * flashDist * flashDist);
        flash = flashDiff * flashAten * 0.6 * lightColor * culoare;
    }

    FragColor = vec4(centerLight + flash, 1.0);
}