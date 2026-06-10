#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;



uniform vec2 uTexSize;
uniform float uRadius;
uniform float uAngle;
uniform vec2 uCenter;
uniform float uTime;


void main()
{
    if(uCenter[0] < 0)
    {
        vec4 color = texture(texture0, fragTexCoord)*colDiffuse*fragColor;
        finalColor = vec4(color.rgb, 1.0);
        return;
    }

    vec2 tc = fragTexCoord*uTexSize;
    tc -= uCenter;

    float dist = length(tc);

    if (dist < uRadius)
    {
        float percent = (uRadius - dist)/uRadius;
        float theta = percent*percent*uAngle*8.0;
        float s = sin(theta);
        float c = cos(theta);

        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
    }

    tc += uCenter;
    vec4 color = texture(texture0, tc/uTexSize)*colDiffuse*fragColor;

    finalColor = vec4(color.rgb, 1.0);
}
