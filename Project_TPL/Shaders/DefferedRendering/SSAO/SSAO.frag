//----------------------------------------------------------------------------------+
// @file        SSAO.frag
// @brief       SSAO Main
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output color
out float FragColor;
//----------------------------------------------------+
// uniform buffer block
// matrices
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
}fs_in;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_texNoise;

uniform vec3 u_samples[64];

// 各種パラメータ
uniform int u_kernelSize = 64;
uniform float u_radius = 0.5f;
float u_bias = 0.025f;
uniform float u_screenW;
uniform float u_screenH;

// 画面の縦横サイズをノイズの大きさで割り、それに基づいてノイズテクスチャを画面上にタイル化する
const vec2 noiseScale = vec2(u_screenW / 4.0f, u_screenH / 4.0f);


void main()
{
	// 各SSAOの入力情報を取得
	vec3 fragPos = texture(u_gPosition, fs_in.fragTexCoords).xyz;
	vec3 normal = normalize(texture(u_gNormal, fs_in.fragTexCoords).rgb);
    vec3 randomVec = normalize(texture(u_texNoise, fs_in.fragTexCoords * noiseScale).xyz);
    // TBN行列の作成:タンジェント空間→ビュー空間
    //vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 tangent = normalize(randomVec - dot(randomVec, normal) * normal);

    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = transpose(mat3(tangent, bitangent, normal));


    // サンプルカーネルを反復し、オクルージョンを算出
    float occlusion = 0.0;
    for(int i = 0; i < u_kernelSize; ++i)
    {
        // サンプリング座標を取得
        vec3 samplePos = u_samples[i] * TBN;                     // タンジェントからビュースペースへ
        samplePos = fragPos + samplePos * u_radius; 
        
        // 座標のサンプリング (テクスチャサンプリング) (スクリーン／テクスチャ上の位置を取得する)
        vec4 offset = vec4(samplePos, 1.0);
        offset = u_projection * offset;                        // ビュー空間からクリップ空間へ
        offset.xyz /= offset.w;                                // パースペクティブの除去
        offset.xyz = offset.xyz * 0.5 + 0.5;                   // レンジ内に変換 → 0.0 - 1.0
        
        // 深度の取得
        float sampleDepth = texture(u_gPosition, offset.xy).z; // カーネルサンプルの深度値を取得
        
        // レンジのチェックと蓄積
        float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + u_bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / u_kernelSize);
    
    FragColor = occlusion;

}