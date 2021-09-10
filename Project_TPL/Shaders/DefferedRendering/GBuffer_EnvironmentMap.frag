//-----------------------------------------------------+
// 環境マッピング (マルチレンダー対応)
//-----------------------------------------------------+
#version 420
// 各バッファへの出力 (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;
layout (location = 3) out vec4 out_gBrightColor;

// 頂点シェーダからの入力
in VS_OUT
{
	vec3 fragNormal;
	vec3 fragWorldPos;
	// 環境マップ用
	vec3 fragEnvNormal;
	vec3 fragEnvWorldPos;
}fs_in;

//----------------------------------------------------+
// uniform buffer block
// camera variables
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};
// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

uniform samplerCube u_skybox;        // キューブマップ(スカイボックス)

uniform float u_luminance = 1.0f;
uniform float u_alpha;

void main()
{
	float ratio = 1.00 / 1.52;      // 反射率
	vec3 I = normalize(fs_in.fragEnvWorldPos - u_viewPos);        // カメラの向きベクトル
	vec3 R = refract(I, normalize(fs_in.fragEnvNormal), ratio);       // カメラの向きベクトルと法線から反射ベクトルを生成

	// 出力カラー
	vec4 resultColor = vec4(texture(u_skybox, R).rgb, u_alpha);

	// GBuffer出力
	out_gPos = fs_in.fragWorldPos;
	out_gNormal = fs_in.fragNormal;
	out_gAlbedoSpec.rgb = resultColor.rgb;
	out_gAlbedoSpec.a = u_alpha;

	if(u_enableBloom == 1)
	{
		out_gBrightColor = vec4(resultColor.rgb, u_alpha) * u_luminance;
	}

}