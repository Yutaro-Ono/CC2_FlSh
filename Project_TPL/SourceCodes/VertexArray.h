//-----------------------------------------------------------------------+
// 頂点クラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once


class VertexArray
{
public:

	//-------------------------------------------------------------------------------------------------------------------+
    // 頂点レイアウトのデータサイズ
    // POS_NORMAL_UV = 8 * sizeof(float) = 32 bytes
    // | POSITION  | NORMAL    | TEXTURE |
    // | x | y | z | x | y | z | u  | v  | 
    //
    // ※タンジェント空間は通常の座標と同じ <3 * sizeof(float) = 12byte>
    //
    // POS_NORMAL_SKIN_UV
    // POS_NORMAL_SKIN_UV = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
    // | POSITION  | NORMAL    | BONES   | WEIGHT  |TEXTURE|
    // | x | y | z | x | y | z | char[4] | char[4] | u | v |
    //                                    ※WEIGHTの確保はcharだが、精度が必要ないので8bit固定小数として使用する
    //-------------------------------------------------------------------------------------------------------------------+
	enum Layout
	{
		POS_NORMAL_TEX,                  // 位置、法線、テクスチャUVを持つフォーマット
		POS_NORMAL_SKIN_TEX,             // 位置、法線、スキン用の影響ボーンと重み情報、テクスチャUV
		POS_NORMAL_TEX_TAN,              // 位置、法線ベクトル、テクスチャUV、法線マップ用タンジェント情報
		POS_NORMAL_SKIN_TEX_TAN          // 位置、法線、スキン用の影響ボーンと重み情報、テクスチャUV, 法線マップ用タンジェント情報
	};

	VertexArray();
	VertexArray(const void* in_verts, unsigned int in_vertsNum, Layout in_layout, const unsigned int* in_inDices, unsigned int in_numInDices);     // 頂点配列コンストラクタ
	VertexArray(const float* in_verts, unsigned int in_vertsNum, const unsigned int* in_inDices, unsigned int in_numInDices);
	VertexArray(const float* in_verts, unsigned int in_vertsNum);
	
	void CreateCubeVerts();                      // スカイボックス用頂点配列オブジェクトの作成
	void CreateScreenVerts();                    // スクリーン用頂点配列オブジェクトの作成
	void CreateSpriteVerts();                    // スプライト用頂点配列オブジェクトの作成

	~VertexArray();                                                        // デストラクタ

	void SetActive();                                                      // この頂点配列をアクティブにして描画で使用する

	unsigned int GetNumIndices() const { return m_numInDices; }            // インデックス数を取得
	unsigned int GetNumVerts() const { return m_vertsNum; }                // 頂点数を取得

private:

	unsigned int m_vertsNum;                                               // 頂点数
	unsigned int m_numInDices;                                             // インデックス数 (ポリゴン面数x3)
	unsigned int m_VAO;                                            // 頂点配列オブジェクトID
	unsigned int m_VBO;                                           // 頂点バッファID (OpenGLに登録時に付与される)
	unsigned int m_indexBuffer;                                              // インデックスバッファID (OpenGL登録時に付与される)
};