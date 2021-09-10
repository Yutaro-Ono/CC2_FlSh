//----------------------------------------------------------------------------------+
// @file        SSAO_Blur.frag
// @brief       Blur effect processing in SSAO
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output color
out float FragColor;

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
}fs_in;

uniform sampler2D u_ssaoInput;


void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_ssaoInput, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_ssaoInput, fs_in.fragTexCoords + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
}