#version 450

layout(binding = 0) uniform UniformBufferObject {
    vec4 cameraPos;
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

struct Particle
{
    vec4 pos;
    vec4 vel;
    vec4 color;
    vec4 ext;
};
layout(set = 0, binding = 2, std430) buffer Particles
{
    Particle particle[];
} particles;

layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

mat4 billboardTowardCameraPlane(vec3 cameraPos)
{
    vec3 look = - normalize(cameraPos);
    vec3 right = normalize(cross(vec3(0,1,0), look));
    vec3 up = normalize(cross(right, look));
    return mat4(
        right.x,right.y,right.z,0,
        up.x,up.y,up.z,0,
        look.x,look.y,look.z,0,
        0,0,0,1);
}

mat4 billboardAlongVelocity(vec3 velocity, vec3 cameraPos)
{
    vec3 up = - normalize(velocity);
    vec3 right = normalize(cross(up, normalize(cameraPos)));
    vec3 look = normalize(cross(right, up));
    return mat4(
        right.x,right.y,right.z,0,
        up.x,up.y,up.z,0,
        look.x,look.y,look.z,0,
        0,0,0,1);
}

float speed_y_curve(float i)
{
    if(i > 1) return 0.769;
    return 0.769 * i;
}
void main() {
    Particle particle = particles.particle[gl_InstanceIndex.x];
    vec3 instance_pos = particle.pos.xyz;

    mat4 billboardMat = billboardAlongVelocity(particle.vel.xyz, ubo.cameraPos.xyz);

    float clamped_speed = clamp(length(particle.vel.xyz), 0, 4) / 4;
    vec4 modelPosition = billboardMat *  ubo.model * vec4(inPosition * vec3(0.2,0.2,0.2) * vec3(0.1, speed_y_curve(clamped_speed), 1),1.0);

    gl_Position = ubo.proj * ubo.view * (vec4(modelPosition.xyz + instance_pos, 1.0));

    float lifeAlpha = 1 - particle.pos.w / particle.ext.x;
    vec4 colorOverLife = texture(texSampler, vec2((0.5 * (lifeAlpha) * 127)/128, 0.75));
    float intensityOverLife = texture(texSampler, vec2((0.5 * (lifeAlpha) * 127)/128, 0.25)).b;
    fragColor = particle.color.rgb *pow(2,intensityOverLife) * colorOverLife.rgb * colorOverLife.a;
    fragTexCoord = inTexCoord;
}