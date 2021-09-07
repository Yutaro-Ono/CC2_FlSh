//-------------------------------------------------------+
// スプライト(2D)用フラグメントシェーダ
//-------------------------------------------------------+
#version 420
// 出力カラー
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;

// 頂点シェーダからの入力
in vec2 FragTexCoords;           // テクスチャ座標

// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

// uniform
uniform sampler2D u_texture;     // テクスチャサンプリング
uniform float u_intensity = 1.0f;  // 輝度調節

void main()
{

	// テクスチャからカラーをサンプリング
	out_color = vec4(texture(u_texture, FragTexCoords).rgb, 1.0f);

	vec3 result = out_color.rgb;

	//out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	// 高輝度バッファへの出力値を抽出
	//float brightness = dot(result, vec3(0.0231, 0.11, 0.65));
	float brightness = dot(result, vec3(0.0, 0.0, 1.0)) * u_intensity;


	if(u_enableBloom == 1)
	{
		if(brightness > 0.5)                                              // 輝度が0.5以上で高輝度バッファへ出力
	    {
		    out_brightColor = out_color * 0.5f;
	    }
	}



}