//-------------------------------------------------------------------------------------------+
//
// 遅延レンダリングクラス (GBufferに各描画結果を保存・GBufferに対して光源処理を行う)
//
//-------------------------------------------------------------------------------------------+
#pragma once
#include <random>
#include <vector>
#include "Math.h"

class DefferedRenderer
{

public:

	DefferedRenderer(class Renderer* in_renderer);
	~DefferedRenderer();

	bool Initialize();                // 初期化処理 (各種フレームバッファを作成する)

	void DrawGBuffer();               // GBufferへの書き込み処理
	void DrawSSAOPath();
	void DrawLightPath();             // GBufferからライティング計算を行う
	void Draw();                      // GBufferに書き込まれた情報の描画


private:

	bool GenerateGBuffer();             // GBufferの作成
	bool GenerateSSAOBuffer();        // SSAOバッファの作成
	bool GenerateLightBuffer();         // ライトバッファの作成

	//-----------------------------+
	// メンバ変数
	//-----------------------------+
	class Renderer* m_renderer;           // レンダラークラスへのポインタ

	// GBuffer
	unsigned int m_gBuffer;               // G-Buffer (3要素を持つフレームバッファ)
    // G-Bufferに関連付ける各種出力
	unsigned int m_gPos;                  // GBuffer用3D空間座標
	unsigned int m_gNormal;               // GBuffer用法線ベクトル
	unsigned int m_gAlbedoSpec;           // GBuffer用アルベド(RGB)＆スペキュラ(A)
	unsigned int m_gEmissive;             // GBuffer用輝度(※エミッシブ)バッファ
	unsigned int m_gAttachments[4];       // GBufferアタッチメント (バッファ数分確保)
	unsigned int m_gRBO;                  // G-Buffer用の描画バッファオブジェクト

	// SSAO
	unsigned int m_ssaoFBO;
	unsigned int m_ssaoBlurFBO;
	// SSAO出力先
	unsigned int m_ssaoColor;
	unsigned int m_ssaoBlurColor;
	// ノイズテクスチャ
	std::vector<Vector3> m_ssaoNoise;
	unsigned int m_noiseTex;
	// カーネルサンプリング用
	std::default_random_engine m_generator;
	std::vector<Vector3> m_ssaoKernel;


	// ライトバッファ (光源処理用)
	unsigned int m_lightFBO;              // ライトバッファ
	// ライトバッファに関連付ける出力
	unsigned int m_lightHDR;              // 光源処理用のHDR対応バッファ (浮動小数点)
	unsigned int m_lightHighBright;       // ライト用高輝度バッファ
	unsigned int m_lightRBO;              // ライト用レンダーバッファ
	unsigned int m_lightAttachments[2];

	// uniformバッファ (GBuffer用)
	unsigned int m_uboGBuffer;

	friend class DefferedRendererDebugObject;
};