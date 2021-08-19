//----------------------------------------------------------------------------------+
// @file        MeshPool.h
// @brief       ���b�V���v�[�� (�e�N�X�`���N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "ObjectPool.h"
#include "Mesh.h"
#include <iostream>

class MeshPool : public ObjectPool<Mesh>
{
public:

	MeshPool();
	~MeshPool();

	void UpdateObjects(float _deltaTime) override;

	class Mesh* GetMesh(const std::string& _meshPath);


};