#version 330
in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
   // ambient
    float Ka = 0.2;
    vec3 ambient = Ka * lightColor * vec3(texture(material.diffuse, TexCoord));
    // diffuse 
	float Kd = 0.7;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = Kd * max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * vec3(texture(material.diffuse, TexCoord));
    // specular
    float Ks = 0.9;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = Ks * spec * lightColor * vec3(texture(material.specular, TexCoord));
    vec3 result = (ambient + diffuse + specular);
    gl_FragColor = vec4(result, 1.0);
}