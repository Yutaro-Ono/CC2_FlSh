//-------------------------------------------------------------------------+
// GBuffer・車のモデル用 フラグメントシェーダ
// シャドウ・環境マップ適用
//-------------------------------------------------------------------------+
#version 420
// GBuffer出力
layout (location = 0) out vec3 out_gPosition;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;
layout (location = 3) out vec4 out_gBrightColor;

// 頂点シェーダからの入力
in VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                // ワールドスペース上の法線
	vec3 fragWorldPos;              // ワールドスペース上の座標
	vec3 fragViewPos;               // カメラ座標
	vec4 fragPosLightSpace;         // ライトスペース上の座標
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
// Directional Light
layout(std140, binding = 3) uniform DirLight
{
	vec3 u_dLightDir;
	vec3 u_dLightDiffuse;
	vec3 u_dLightSpecular;
	vec3 u_dLightAmbient;
	float u_dLightIntensity;
};

// マテリアル構造体
struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D depthMap;
	sampler2D emissiveMap;
};


// 各種構造体 (uniform)
uniform Material u_mat;

uniform samplerCube u_skybox;     // サンプリング用キューブマップ

uniform float u_specPower;

// シャドウの計算
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // パースペクティブ除算(遠近感を出すための計算だが、今回はw成分が1のため意味なし)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 正規化デバイス座標をテクスチャ座標と合わせるため、[-1～1]を[0～1]の範囲に変換
    projCoords = projCoords * 0.5 + 0.5;
    // シャドウマップよりライトに最も近いフラグメントの深度値を得る
    float closestDepth = texture(u_mat.depthMap, projCoords.xy).r;
    // 現在描画しようとしているフラグメントの深度値
    float currentDepth = projCoords.z;
    // シャドウ判定 (1.0:シャドウ 0.0:シャドウの外)
    // バイアスを法線とライトの向きから調整する
    float bias = max(0.0001 * (1.0 - dot(normalize(fs_in.fragNormal), u_dLightDir)), 0.0001);
    // 現在の深度が最も近いフラグメントの深度より大きければ1.0、小さければ0.0(影になる)
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(u_mat.depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_mat.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    // PhongLighting
	vec3 N = normalize(fs_in.fragNormal);                    // Polygon Surface normal
	vec3 L = normalize(-u_dLightDir);                        // Vector from : Neg Light Direction
	vec3 V = normalize(u_viewPos - fs_in.fragWorldPos);      // Vector from : Polygon Pos -> Camera Pos
	vec3 R = normalize(reflect(-L, N));                      // Reflect Vector from : Light Dir -> Polygon Surface
	// Phong Reflection Calculation
	vec3 Phong = u_dLightAmbient;
	float NdotL = dot(N, L);
	// 環境マップサンプリング
	float ratio = 1.00 / 1.00;                                        // 反射率
	vec3 I = normalize(fs_in.fragWorldPos - fs_in.fragViewPos);        // カメラの向きベクトル
	vec3 eR = refract(I, normalize(fs_in.fragNormal), ratio);                 // カメラの向きベクトルと法線から反射ベクトルを生成
	vec3 envMap = texture(u_skybox, eR).rgb;

	// ディフューズ計算
	vec3 color = texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;
	vec3 Diffuse = u_dLightDiffuse;
	// スペキュラ計算
	vec3 Specular = u_dLightSpecular * pow(max(0.0, dot(R, V)), u_specPower) * texture(u_mat.specularMap, fs_in.fragTexCoords).rgb;
	//vec3 Specular = u_dLightSpecular * texture(u_mat.specularMap, fs_in.fragTexCoords).rgb;

	// アンビエント
	vec3 ambient = u_dLightAmbient * color;

	// 影成分の算出
	float shadow = ShadowCalculation(fs_in.fragPosLightSpace);

	// GBuffer出力
	//out_gPosition = vec3(fs_in.fragWorldPos.z, fs_in.fragWorldPos.x, -fs_in.fragWorldPos.y);
	out_gPosition = fs_in.fragWorldPos;
	out_gNormal = fs_in.fragNormal;
	// シャドウの逆数を取り、0 = 影の時にディフューズとスペキュラの値がキャンセルされる(影となる)
	//out_gAlbedoSpec = vec4((ambient + (0.8f - shadow)) * (Diffuse + Specular + envMap), Specular.r);
	out_gAlbedoSpec = vec4((Diffuse + ((ambient + Specular) * (0.8f - shadow)) + envMap), Specular.r);

	if(u_enableBloom == 1)
	{
	    out_gBrightColor = vec4(envMap, 1.0f) + texture(u_mat.emissiveMap, fs_in.fragTexCoords) * 0.095f;     // 0.03f
	}
}