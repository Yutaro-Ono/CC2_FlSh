//----------------------------------------------------------------------------------+
// @file        Renderer.h
// @brief       描画処理
//              メッシュ・テクスチャ・シェーダー情報の保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   新規作成
// 2021/ 5/26   各種バッファを登録 (Gバッファ・ライト・MSAA)
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_syswm.h>
/*
#include <Effekseer.h>
#include <EffekseerRendererGL.h>
*/
#include <Windows.h>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "Math.h"

// 描画モード（正面・遅延）
enum class RENDER_MODE : unsigned int
{
	FORWARD = 0,
	DEFFERED
};


class Renderer
{

public:


	Renderer();
	~Renderer();

	bool Initialize(int in_screenW, int in_screenH, bool in_full);          // 各種初期化処理 (GL SDL)
	bool Load();                                                            // ロード関数
	void Delete();                                                          // 各種解放処理
	void Draw();                                                            // 描画

	// SpriteComponent
	void AddSprite(class SpriteComponent* in_sprite);
	void RemoveSprite(class SpriteComponent* in_sprite);
	// Sprite (ワールド空間)
	void AddSpriteWorld(class WorldSpaceUI* in_sprite);
	void RemoveSpriteWorld(class WorldSpaceUI* in_sprite);
	// MeshComponent
	void AddMeshComponent(class MeshComponent* in_mesh);                    // メッシュコンポーネントの追加
	void RemoveMeshComponent(class MeshComponent* in_mesh);                 // メッシュコンポーネントの削除
	// CarMeshComponent
	void AddCarMeshComponent(class CarMeshComponent* in_carMesh);
	void RemoveCarMeshComponent(class CarMeshComponent* in_carMesh);
	// EnvironmentMapComponent
	void AddEnvironmentComponent(class EnvironmentMapComponent* in_envMesh);
	void RemoveEnvironmentComponent(class EnvironmentMapComponent* in_envMesh);
	// LightGlassComponent
	void AddLightGlassComponent(class LightGlassComponent* in_glassMesh);
	void RemoveLightGlassComponent(class LightGlassComponent* in_glassMesh);
	// PointLightComponent
	void AddPointLightComponent(class PointLightComponent* in_pointL);
	void RemovePointLightComponent(class PointLightComponent* in_pointL);
	// SpotLightComponent
	void AddSpotLightComponent(class SpotLightComponent* in_spotL);
	void RemoveSpotLightComponent(class SpotLightComponent* in_spotL);
	// MapHUD
	void RemoveMapHUD();

	// TextureComponent
	void RemoveTexture(class Texture* in_texture);

	// ShadowMap
	class ShadowMap* GetShadowMap() { return m_shadowMap; }

	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void WindowFlip() { SDL_GL_SwapWindow(m_window); }

	void SetWindowTitle(const std::string& in_title);
	void SetWorldSpriteVertex();
	void SetDirectionalLight(class DirectionalLight* _dirLight) { m_dirLight = _dirLight; }
	void SetActiveSpriteVAO();                                            // スプライト頂点配列のアクティブ化

	void SetActiveLineVAO();
	void SetActiveBoxVAO();
	void SetActiveSquareVAO();

	void UpdateUBO();

	//--------------------------------------------+
	// Getter / Setter
	//--------------------------------------------+
	SDL_Renderer* GetSDLRenderer() { return m_SDLRenderer; }                // SDL刑の描画に用いるSDLRendererのゲッター
	SDL_Window* GetSDLWindow() { return m_window; }                         // SDL Windowのゲッター
	SDL_GLContext GetSDLGLContext() { return m_context; }                   // SDL_GLコンテキストのゲッター
	HWND GetHWND() { return m_hwnd; }                                       // HWNDのゲッター (ハンドルコントローラの初期化に使用)
	
	class Texture* GetTexture(const std::string& in_fileName);              // 指定したファイル名のテクスチャを返す
	class Mesh* GetMesh(const  std::string& in_fileName);                   // 指定したファイル名のメッシュを返す
	const class Skeleton* GetSkeleton(const std::string& in_fileName);             // 指定したファイル名のスケルタルメッシュの取得
	const class Animation* GetAnimation(const char* in_fileName, bool in_loop);
	// スクリーンサイズ
	float GetScreenWidth() { return static_cast<float>(m_screenWidth); }
	float GetScreenHeight() { return static_cast<float>(m_screenHeight); }
	// ディレクショナルライト
	class DirectionalLight* GetDirectionalLight() { return m_dirLight; }
	// カメラ位置セット
	void SetCameraPosition(const Vector3& in_cameraPos) { m_cameraPos = in_cameraPos; }
	// 行列
	const Matrix4& GetViewMatrix() { return m_viewMat; }
	const Matrix4& GetProjectionMatrix() { return m_projMat; }
	// ParticleManager
	class ParticleManager* GetParticleManager() const;

	void SetViewMatrix(const Matrix4& in_view) { m_viewMat = in_view; }
	void SetAmbientLight(const Vector3& in_ambientColor) { m_ambientLight = in_ambientColor; }
	const Vector3 GetAmbientLight() { return m_ambientLight; }
	void SetProjectionMatrix(const Matrix4& in_proj) { m_projMat = in_proj; }

	// アクティブスカイボックス
	void SetActiveSkyBox(class CubeMapComponent* in_skyBox) { m_activeSkyBox = in_skyBox; }
	class CubeMapComponent* GetSkyBox() { return m_activeSkyBox; }
	class VertexArray* GetCubeMapVerts() { return m_cubeVerts; }

	// スクリーン用頂点配列クラスのゲッター
	class VertexArray* GetScreenVAO() { return m_screenVerts; }

	// ミニマップのセッター・ゲッター
	void SetMapHUD(class MiniMapHUD* in_mapHUD);
	class MiniMapHUD* GetMapHUD() { return m_mapHUD; }
	// ブルームクラスゲッター
	class RenderBloom* GetBloom() { return m_bloom; }

	// 描画モード
	RENDER_MODE GetRenderMode() { return m_renderMode; }
	void SetRenderMode(RENDER_MODE _renderMode) { m_renderMode = _renderMode; }
	// シェーダー管理クラスのゲッター
	class ShaderManager* GetShaderManager() { return m_shaderManager; }

	/*
	// Effekseer関連のゲッター
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> GetEffekseerRenderer() { return m_effekseerRenderer; }
	Effekseer::RefPtr<Effekseer::Manager> GetEffekseerManager() { return m_effekseerManager; }
	*/
private:

	void CreateUBOs();
	bool LoadShaders();                                                    // シェーダーの初期化

	int m_screenWidth, m_screenHeight;                                     // ウィンドウの横幅、縦幅
	bool m_fullScreen;                                                     // フルスクリーンかどうか

	std::unordered_map<std::string, class Texture*> m_textures;            // テクスチャ登録配列
	std::unordered_map<std::string, class Mesh*> m_meshes;                 // メッシュ登録配列
	
	// スケルトン・アニメーションの登録用配列(スケルタルメッシュのみ)
	std::unordered_map<std::string, class Skeleton*> m_skeletons;
	std::unordered_map<std::string, class Animation*> m_animations;

	// メッシュ配列
	std::vector<class MeshComponent*> m_meshComponents;                       // メッシュコンポーネント
	std::vector<class SkeletalMeshComponent*> m_skeletalMeshComponents;       // ボーン入りメッシュ配列
	std::vector<class CarMeshComponent*> m_carMeshComponents;                 // 車メッシュ配列
	std::vector<class EnvironmentMapComponent*> m_envMeshComponents;          // 環境マップオブジェクト配列
	std::vector<class LightGlassComponent*> m_lightGlassComponents;           // ライトグラスコンポーネント配列
	// スプライト配列
	std::vector<class SpriteComponent*> m_spriteComponents;                   // スプライト配列 (スクリーン空間)
	std::vector<class WorldSpaceUI*> m_worldSprites;                          // スプライト配列 (ワールド空間)
	

	// ライト関連配列
	std::vector<class PointLightComponent*> m_pointLights;                           // ポイントライト配列
	std::vector<class SpotLightComponent*> m_spotLights;                             // スポットライト配列

	// キューブマップ
	class CubeMapComponent* m_activeSkyBox;                                   // 有効な(描画する)スカイボックス
	// マップHUD
	class MiniMapHUD* m_mapHUD;

	//---------------------------------------------------+
	// uniformバッファ (uniform変数の格納用バッファ)
	//---------------------------------------------------+
	unsigned int m_uboMatrices;                  // ビュー・プロジェクション行列用UBO
	unsigned int m_uboCamera;                    // カメラ情報
	unsigned int m_uboTriggers;                  // トリガー類(ブルーム効果のオンオフ)
	unsigned int m_uboDirLights;                 // ディレクショナルライトUBO

	//--------------------------------------------+
	// 頂点配列
	//--------------------------------------------+
	class VertexArray* m_spriteVerts;                                      // スプライト用頂点配列
	class VertexArray* m_particleVerts;                                    // パーティクル用頂点配列
	class VertexArray* m_cubeVerts;                                        // キューブ頂点配列
	class VertexArray* m_screenVerts;                                      // スクリーン全体を覆う四角形
	class VertexArray* m_lineVerts;                                        // ライン(線分)用の頂点配列
	class VertexArray* m_boxVerts;                                         // ライン(ボックス)用の頂点配列
	class VertexArray* m_squareVerts;                                      // ライン(四角形)用の頂点配列


	//--------------------------------------------+
	// ライティング関連
	//--------------------------------------------+
	Vector3 m_ambientLight;                                                // アンビエントライト
	class DirectionalLight* m_dirLight;

	//--------------------------------------------+
	// シャドウ関連
	//--------------------------------------------+
	class ShadowMap* m_shadowMap;

	//--------------------------------------------+
    // HDR・Bloom関連
    //--------------------------------------------+
	class RenderBloom* m_bloom;
	int m_switchShader;

	//--------------------------------------------+
	// パーティクル関連
	//--------------------------------------------+
	class ParticleManager* m_particleManager;

	//--------------------------------------------+
	// ポストエフェクト関連
	//--------------------------------------------+
	class PostEffect* m_postEffect;

	//--------------------------------------------+
    // カメラ関連
    //--------------------------------------------+
	Vector3 m_cameraPos;

	//--------------------------------------------+
	// 基本行列関連
	//--------------------------------------------+
	Matrix4 m_viewMat;
	Matrix4 m_projMat;       // プロジェクション(視錐台)

	//--------------------------------------------+
	// アプリケーション情報関連
	//--------------------------------------------+
	SDL_Window* m_window;                                                   // SDLのウィンドウハンドル
	SDL_Renderer* m_SDLRenderer;                                            // SDLのレンダリングハンドル
	SDL_GLContext m_context;                                                // OpenGLコンテキスト (内部状態管理)
	HWND m_hwnd;                                                            // ウィンドウハンドル  

	// フォワード・ディファードレンダリングクラス (描画はどちらかで行う)
	class ForwardRenderer* m_fRenderer;
	class DefferedRenderer* m_dRenderer;
	RENDER_MODE m_renderMode;             // レンダリングモード (フォワードかディファードか)

	//--------------------------------------------+
	// シェーダ管理
	//--------------------------------------------+
	class ShaderManager* m_shaderManager;

	/*
	// Effekseer
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> m_effekseerRenderer;    // Effekseer用レンダラー
	Effekseer::RefPtr<Effekseer::Manager> m_effekseerManager;                // Effekseer用マネージャー
	*/

	// フラグ関連
	bool m_enableBloom;
	bool m_visualizeNormal;

	// フレンドクラス (フォワード・ディファードクラスはレンダラーに直接アクセス可能)
	friend class ForwardRenderer;
	friend class DefferedRenderer;
	friend class RendererDebugObject;
};