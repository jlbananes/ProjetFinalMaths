uniform mat4 mvpMatrix;
uniform lowp float t;
//uniform vec4 color;

//varying highp vec2 coords;

//in vec3 in_color;
in vec4 vertex;

varying vec3 color;

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
    //coords = vertex.xy;
    //color = in_color;
}
