#version 330

//uniform vec4 normals;
//uniform vec4 color;
//uniform lowp float t;
//varying highp vec2 coords;

smooth in vec4 color;
smooth in vec4 texCoord;
smooth in vec3 normal;

uniform sampler2D texture;

out vec4 out_color;
lowp float lightIntensity;

void main(void)
{
    /*lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));
    i = smoothstep(0.8, 0.8, 0.8);
    i = floor(i * 20.) / 20.;
    gl_FragColor = vec4(coords * .5 + .5, i, i);*/     // test faces multicolores
    //out_color = vec4(color, 1.0);               // test faces colorées unies

    vec3 lightDirection = vec3(0.8,0.2,0.5);
    lightIntensity = 4.0 * pow(dot(normalize(normal),normalize(lightDirection)),2);

    out_color = vec4(color.rgb * lightIntensity, color.a);
    //out_color = texture2D(texture, vec2(texCoord.st));
    //gl_FragColor = color;                         // test faces jaunes
}
