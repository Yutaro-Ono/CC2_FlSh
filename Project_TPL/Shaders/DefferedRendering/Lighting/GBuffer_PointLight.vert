//----------------------------------------------------------------------------------+
// @file        GBuffer_PointLight.vert
// @brief       Lighting Pass of PointLight(Deffered)
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_pos;

// uniformバッファブロック
// 0.行列
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};
// 1.カメラ座標
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};

// out structure (Output to Fragment)
out VS_OUT
{
	vec2 fragTexCoords;
}vs_out;

uniform mat4 u_worldTransform;

void main()
{
	vec4 pos = u_projection * u_view * u_worldTransform * vec4(a_pos, 1.0f);

	gl_Position = pos;

	// conversion : Normalized device coordinates -> Texture coordinates of gBuffer
	// x / w, y / w, -> [0.0 ~ 1.0]
	vs_out.fragTexCoords.x = pos.x / pos.w * 0.5f + 0.5f;
	vs_out.fragTexCoords.y = pos.y / pos.w * 0.5f + 0.5f;
}