//-----------------------------------------------------------------------+
// メッシュコンポーネントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "MeshComponent.h"
#include "GLSLprogram.h"
#include "Mesh.h"
#include "Actor.h"
#include "GameMain.h"
#include "Renderer.h"
#include "ShadowMap.h"
#include "Texture.h"
#include "VertexArray.h"
#include <typeinfo>

// コンストラクタ
MeshComponent::MeshComponent(Actor * _owner, bool _isSkeletal)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_textureIndex(0)
	,m_visible(true)
	,m_isDrawMap(true)
	,m_isSkeletal(_isSkeletal)
	,m_intensity(1.0f)
	,m_mapColor(Vector3(0.5f, 0.5f, 0.5f))
{
	RENDERER->AddMeshComponent(this);
	//printf("new MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), _owner);
}

// デストラクタ
MeshComponent::~MeshComponent()
{
	//printf("Remove MeshComponent : [%5d] owner->( 0x%p )\n)", GetID(), m_owner);
	RENDERER->RemoveMeshComponent(this);
}

// 通常描画処理
void MeshComponent::Draw(GLSLprogram * _shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ワールド変換をセット
		_shader->SetUniform("u_worldTransform", m_owner->GetWorldTransform());
		// スペキュラ強度セット
		_shader->SetUniform("u_specPower", 32);
		// 輝度強度セット
		_shader->SetUniform("u_intensity", m_intensity);

		SetTexturesToUniform(_shader);          // Uniformに各種テクスチャをセット

		// 頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

// シャドウ生成用の描画
void MeshComponent::DrawShadow(GLSLprogram* _shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ワールド変換をセット
		_shader->SetUniform("u_worldTransform", m_owner->GetWorldTransform());

		// 頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void MeshComponent::DrawMap(GLSLprogram* _shader)
{
	if (m_mesh != nullptr && m_isDrawMap)
	{
		// ワールド変換をセット
		_shader->SetUniform("u_worldTransform", m_owner->GetWorldTransform());
		// スペキュラ強度セット
		_shader->SetUniform("u_mapColor", m_mapColor);

		// 頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

/// <summary>
/// シェーダuniformに各種テクスチャをセットする関数
/// </summary>
/// <param name="_shader"> シェーダクラスのポインタ </param>
void MeshComponent::SetTexturesToUniform(GLSLprogram* _shader)
{
	// ディフューズ → スペキュラ → ノーマル → エミッシブ → シャドウ の順でセット
	// 指定タイプのテクスチャをMeshが保持していなかった場合、無効数字 "0"がセット
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::DIFFUSE_MAP));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::SPECULAR_MAP));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::NORMAL_MAP));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::EMISSIVE_MAP));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, RENDERER->GetShadowMap()->GetDepthMap());
}

