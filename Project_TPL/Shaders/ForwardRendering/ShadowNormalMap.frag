#version 330 core
// 出力カラー（出力ピクセルカラー）
out vec4 out_fragColor;

// 頂点シェーダーからの入力受け取り
in VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                // 法線（ワールド空間）
	vec3 fragWorldPos;              // 頂点位置（ワールド空間）
	vec3 fragViewPos;               // カメラ座標
	vec4 fragPosLightSpace;         // 頂点位置 (ライト空間)
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;

}fs_in;


// テクスチャサンプリング用構造体
struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D depthMap;
};
uniform Material u_mat;

// ディレクショナルライト用構造体
struct DirectionalLight
{
	vec3 direction;      // ライト方向
	vec3 ambient;        // アンビエント
	vec3 diffuse;        // ディフューズ色
	vec3 specular;       // スペキュラー色
};
// ディレクショナルライト
uniform DirectionalLight u_dirLight;


// ポリゴン表面のスペキュラー強度
uniform float u_specPower;


// シャドウの計算
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // パースペクティブ除算(遠近感を出すための計算だが、今回はw成分が1のため意味なし)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 正規化デバイス座標をテクスチャ座標と合わせるため、[-1〜1]を[0〜1]の範囲に変換
    projCoords = projCoords * 0.5 + 0.5;
    // シャドウマップよりライトに最も近いフラグメントの深度値を得る
    float closestDepth = texture(u_mat.depthMap, projCoords.xy).r;
    // 現在描画しようとしているフラグメントの深度値
    float currentDepth = projCoords.z;
    // シャドウ判定 (1.0:シャドウ 0.0:シャドウの外)
    // バイアスを法線とライトの向きから調整する
    float bias = max(0.0001 * (1.0 - dot(normalize(fs_in.fragNormal), u_dirLight.direction)), 0.0001);
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

	// 範囲[0, 19の法線マップベクトルを取得
	vec3 normal = texture(u_mat.normalMap, fs_in.fragTexCoords).rgb;
	// 法線ベクトルの範囲を[-1, +1]の範囲に復元する(タンジェントスペースに変換)
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;

	// アンビエント計算
	vec3 ambient = u_dirLight.ambient * color;


	// ディフューズ計算
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = u_dirLight.diffuse * diff * texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;


	// スペキュラ計算
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = u_dirLight.specular * spec * texture(u_mat.specularMap, fs_in.fragTexCoords).rgb;

	// 影成分の算出
	float shadow = ShadowCalculation(fs_in.fragPosLightSpace);
	// シャドウの逆数を取り、0 = 影の時にディフューズとスペキュラの値がキャンセルされる(影となる)
	vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);


	// 最終カラーを出力
	out_fragColor = vec4(result, 1.0);


}
