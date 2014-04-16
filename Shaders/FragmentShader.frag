//http://www.youtube.com/user/thecplusplusguy
//http://www.youtube.com/watch?v=MRD_zN0SWh0

//uniform vec4 normals;
uniform vec4 color;
//uniform lowp float t;
//varying highp vec2 coords;

//in vec4 varyingColor;
//in vec3 color;

void main(void)
{
    /*lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));
    i = smoothstep(0.8, 0.8, 0.8);
    i = floor(i * 20.) / 20.;
    gl_FragColor = vec4(coords * .5 + .5, i, i);*/
    //gl_FragColor = vec4(color, 1.0f);
    gl_FragColor = color;
}
