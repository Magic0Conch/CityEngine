#version 330 core
out vec4 FragColor;


in VS_OUT{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 lightColor;
}fs_in;

uniform sampler2D texture1;
uniform sampler2D normalmap;
uniform sampler2D depthMap;
uniform float height_scale;

vec3 blingPhone(vec2 texCoord){
    vec3 ansColor;
    vec3 ambientColor = fs_in. lightColor*0.03;

    vec3 normal = normalize( vec3(texture(normalmap,texCoord)));
    normal = normalize((normal*2.0)-1.0);

    vec3 frag2lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diffuseCoefficient = max(dot(normal,frag2lightDir),0.0);
    vec3 diffuseColor = diffuseCoefficient * fs_in.lightColor;
    

    vec3 frag2viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 h = normalize(frag2viewDir + frag2lightDir);
    float specularCoefficient = max(dot(normal,h),0.0);
    vec3 specularColor = pow(specularCoefficient,128) * fs_in.lightColor;

    float dist = distance(fs_in.FragPos,fs_in.TangentLightPos);
    float attenunation = 1.0/(1 + dist*0.09 + dist*dist*0.032);

    ansColor = ambientColor + (diffuseColor + specularColor)*attenunation;

    return ansColor;
}

vec2 ParallexMapping(vec2 texCoord, vec3 viewDir){
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float layers = 10;
    float layerDepth = 1.0/layers;
    float currentLayerDepth = 0;
    float currentDepthMapValue = texture(depthMap,texCoord).r;
    vec2 p = viewDir.xy * ( height_scale);
    vec2 deltaTexCoord = p/layers;



    while(currentLayerDepth<currentDepthMapValue){
        currentLayerDepth+=layerDepth;
        texCoord-=deltaTexCoord;
        currentDepthMapValue = texture(depthMap,texCoord).r;
    }
    vec2 prevTexCoord = texCoord + deltaTexCoord;
    float prevDepthMapValue = texture(depthMap,prevTexCoord).r;
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth =  (currentLayerDepth - layerDepth)-prevDepthMapValue;
    float weight = afterDepth/(afterDepth+beforeDepth);

    return prevTexCoord*weight + texCoord*(1.0-weight);

    // float height = texture(depthMap,texCoord).r;
    //divide by viewDir.z: the more parallel,the larger p
    // return texCoord - p;
}

void main()
{             
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoord =  ParallexMapping(fs_in.TexCoords,viewDir);
    if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
        discard;
    vec3 texColor = vec3(texture(texture1,texCoord));
    FragColor = vec4(texColor * blingPhone(texCoord),1.0);
}