//---------------------------------------------+
// 環境マッピング (マルチレンダー対応)
//---------------------------------------------+
#version 420
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

// uniformバッファブロック (行列)
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

// 出力
// フラグメントへの出力
out VS_OUT
{
	vec3 fragNormal;                // ワールドスペース上の法線
	vec3 fragWorldPos;              // ワールドスペース上の座標
	// 環境マップ用
	vec3 fragEnvNormal;
	vec3 fragEnvWorldPos;
}vs_out;

// 入力
uniform mat4 u_worldTransform;
uniform mat4 u_offset;

void main()
{
	vec4 pos = u_worldTransform * vec4(a_pos, 1.0);
	gl_Position = u_projection * u_view * pos;

	vs_out.fragNormal = mat3(transpose(u_worldTransform)) * a_normal;
	vs_out.fragWorldPos = pos.xyz;                                                 // ワールド上の位置ベクトルを出力

	// 環境マップ用ノーマル+ワールド座標
	vs_out.fragEnvNormal = vs_out.fragNormal;
	vs_out.fragEnvNormal = mat3(u_offset) * vs_out.fragEnvNormal;
	vs_out.fragEnvWorldPos = pos.xyz;
}