#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
#define M_PI 3.14159265358979323846
out vec3 ourColor;
uniform float xOffset;
vec3 temo_aPos;
float de;
void main()
{
    temo_aPos = aPos;
    de = radians(xOffset);
    temo_aPos = vec3((temo_aPos.x-0.4330125)*cos(xOffset) - (temo_aPos.y+0.25)*sin(xOffset)+0.4330125,(temo_aPos.x-0.4330125)*sin(xOffset) + (temo_aPos.y+0.25)*cos(xOffset)-0.25,aPos.z);
    gl_Position = vec4(temo_aPos.x,temo_aPos.y,temo_aPos.z, 1.0);
    ourColor = aColor;
}