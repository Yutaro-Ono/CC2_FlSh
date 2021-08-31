//-------------------------------------------------------------------------+
// GBuffer・車のモデル用 頂点シェーダ
// 環境マップ・反射計算
//-------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

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

// ワールド変換行列・ライトスペース
uniform mat4 u_worldTransform;
uniform mat4 u_lightSpaceMatrix;   
// 光源座標
uniform vec3 u_lightPos;
// 反射マップオフセット(映り込みの角度調整用)
uniform vec3 u_offset;

// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                // ワールドスペース上の法線
	vec3 fragWorldPos;              // ワールドスペース上の座標
	vec3 fragViewPos;               // カメラ座標
	vec4 fragPosLightSpace;         // ライトスペース上の座標
}vs_out;


void main()
{
	vec4 pos =  u_worldTransform * vec4(a_pos, 1.0);
	gl_Position = u_projection * u_view * pos;
	vs_out.fragWorldPos    = pos.xyz;
	vs_out.fragNormal      = mat3(transpose(inverse(u_worldTransform))) * a_normal;
	vs_out.fragNormal      = vec3(vs_out.fragNormal.y, -vs_out.fragNormal.z, vs_out.fragNormal.x);
	vs_out.fragTexCoords   = a_texCoords;
	vs_out.fragViewPos     = u_viewPos;
	// ワールド座標の頂点をライトスペースに変換して保存
	vs_out.fragPosLightSpace = u_lightSpaceMatrix * vec4(vs_out.fragWorldPos, 1.0);

	//vs_out.fragNormal = vec3(vs_out.fragNormal.y, -vs_out.fragNormal.z, vs_out.fragNormal.x);
	//vs_out.fragWorldPos = vec3(pos.y, -pos.z, pos.x);                                                 // ワールド上の位置ベクトルを出力
}