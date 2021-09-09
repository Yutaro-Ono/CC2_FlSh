//----------------------------------------------------+
// スキン入りメッシュ用頂点シェーダ
//----------------------------------------------------+
#version 420
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;
layout(location = 5) in vec3 a_tangent;

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
//
// 変換行列 (ワールド、ビュー、プロジェクション)
uniform mat4 u_worldTransform;
// 行列パレット
uniform mat4 u_matrixPalette[196];
// ライト空間行列
uniform mat4 u_lightSpaceMatrix;
// 光源座標・カメラ座標
uniform vec3 u_lightPos;


// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;            // ワールドスペース上の法線
	vec3 fragWorldPos;          // ワールドスペース上の座標
	vec3 fragViewPos;           // ビュー座標
	vec4 fragPosLightSpace;     // ライトスペース上の座標
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;

}vs_out;

void main()
{
	// 頂点座標を四次元に変換
	vec4 pos = vec4(a_pos, 1.0);
	
	// テクスチャ座標を出力
	vs_out.fragTexCoords = a_texCoords;

	// スキン位置を算出
	vec4 skinnedPos = a_skinWeights.x * (pos * u_matrixPalette[a_skinBones.x]);
	skinnedPos += a_skinWeights.y * (pos * u_matrixPalette[a_skinBones.y]);
	skinnedPos += a_skinWeights.z * (pos * u_matrixPalette[a_skinBones.z]);
	skinnedPos += a_skinWeights.w * (pos * u_matrixPalette[a_skinBones.w]);
	// ワールド上のスキン位置を保存
	vs_out.fragWorldPos = (u_worldTransform * skinnedPos).xyz;
	// クリップ空間へ頂点を変換し出力に渡す
	gl_Position = u_projection * u_view * u_worldTransform * skinnedPos;


	// スキン位置の法線を算出
	vec4 skinnedNormal = vec4(a_normal, 0.0f);
	skinnedNormal = (u_matrixPalette[a_skinBones.x] * skinnedNormal) * a_skinWeights.x
		+ (u_matrixPalette[a_skinBones.y] * skinnedNormal) * a_skinWeights.y
		+ (u_matrixPalette[a_skinBones.z] * skinnedNormal) * a_skinWeights.z
		+ (u_matrixPalette[a_skinBones.w] * skinnedNormal) * a_skinWeights.w;
	// 法線をワールド空間上に変換(w = 0)
	vs_out.fragNormal = mat3(transpose(inverse(u_worldTransform))) * a_normal;

	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

	//------------------------------------------------------------------------------------+
	// Tangent計算
	vec3 T, B, N;     // TBN行列
	N = a_normal;     // 法線を代入
	T = a_tangent;    // 接空間(TangentSpace)を代入

	// 法線に対する接空間の再直行化
	T = normalize(T - dot(T, N) * N);
	// 接空間ベクトルと法線ベクトルの外積から垂直ベクトルB(BiTangent)を取得
	B = cross(N, T);

	T = normalize(vec3(u_worldTransform * vec4(T, 0.0f)));
	B = normalize(vec3(u_worldTransform * vec4(B, 0.0f)));
	N = normalize(vec3(u_worldTransform * vec4(N, 0.0f)));

	// TBN行列を逆行列として生成
	mat3 TBN = transpose(mat3(T, B, N));

	// 接空間内の座標定義
	vs_out.TangentLightPos = TBN * u_lightPos;                                     // 接空間における光源位置
	vs_out.TangentViewPos = TBN * u_viewPos;                                       // 接空間におけるビュー座標
	vs_out.TangentFragPos = TBN * vec3(u_worldTransform * vec4(skinnedPos.xyz, 0.0f));      // 接空間における頂点座標
}

