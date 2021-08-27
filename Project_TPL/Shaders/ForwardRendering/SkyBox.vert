//-------------------------------------------------------+
// スカイボックス用頂点シェーダ
//-------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_pos;

out vec3 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_invTransView;
uniform mat4 u_offset;

void main()
{
	// テクスチャ座標にローカル位置ベクトルを設定
	TexCoords = a_pos;

	vec4 pos = vec4(a_pos, 1.0) * u_offset * u_invTransView * u_projection;

	gl_Position = pos.xyww;     // z成分をwで置き換える(最奥表示)

}