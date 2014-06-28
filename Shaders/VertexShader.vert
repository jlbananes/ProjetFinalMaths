#version 330

uniform mat4 mvpMatrix;
uniform lowp float t;

//varying highp vec2 coords;

in vec4 vertex;
in vec4 in_color;
in vec3 in_normal;
in vec2 in_texCoord;

smooth out vec4 color;
smooth out vec3 normal;
smooth out vec4 texCoord;

//float PI = 3.1415926535897932384626433832795;
//lowp float a = PI/4;

mat4 rotationX = mat4(
            1.0, 0.0, 0.0, 0.0,
            0.0, cos(t), sin(t), 0.0,
            0.0, -sin(t), cos(t), 0.0,
            0.0, 0.0, 0.0, 1.0);
mat4 rotationY = mat4(
            cos(t), 0.0, sin(t), 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sin(t), 0.0, cos(t), 0.0,
            0.0, 0.0, 0.0, 1.0);

void main(void)
{
    mat4 rotationXY = rotationX * rotationY;
    gl_Position = mvpMatrix * rotationXY * vertex;
    //texCoord = vec4(in_texCoord,vec2(0.0,0.0));
    //vec4 newNormal = mvpMatrix * rotationXY * vec4(in_normal,1.0);
    //normal = vec3(newNormal.xyz);
    //gl_Normal = in_normal;
    color = in_color;
}
