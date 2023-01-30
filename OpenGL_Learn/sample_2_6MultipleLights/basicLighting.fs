#version 330 core
out vec4 FragColor;
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 
uniform Material material; 
struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
struct SpotLight
{
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;
in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
uniform vec3 viewPos; 

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
void main()
{
    // ����
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // ��һ�׶Σ��������
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // �ڶ��׶Σ����Դ
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
         result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // �����׶Σ��۹�
   result += CalcSpotLight(spotLight, norm, FragPos, viewDir);   

   FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    //������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    //����
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear*distance+light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    //������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    //����
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
     
    ambient *= attenuation;
    diffuse *= attenuation;
    reflectDir *= attenuation;
     
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   

    // ִ�й��ռ���
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    //������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    //����
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      
    // �����Ի���������Ӱ�죬������������һ���
    ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);

      
}