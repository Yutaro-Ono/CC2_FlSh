//-----------------------------------------------------------------------+
// ���b�V���R���|�[�l���g�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"

class MeshComponent : public Component
{
public:

	MeshComponent(class Actor* _owner, bool _isSkeletal = false);       // �R���X�g���N�^(�e�A�N�^�[���K�v)
	~MeshComponent();                                                       // �f�X�g���N�^

	virtual void Draw(class GLSLprogram* _shader);                             // ���b�V���R���|�[�l���g�̕`��
	virtual void DrawShadow(class GLSLprogram* _shader);                       // �V���h�E�����p�̕`��
	void DrawMap(class GLSLprogram* _shader);                                  // �}�b�vHUD�`��p

	virtual void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }         // ���b�V���R���|�[�l���g�Ŏg�p���郁�b�V���̃Z�b�g

	void SetTexturesToUniform(class GLSLprogram* _shader);                     // �V�F�[�_uniform�Ƀe�N�X�`�����Z�b�g����֐�
	void SetTextureIndex(size_t _index) { m_textureIndex = _index; }    // �e�N�X�`���̃Z�b�g

	void SetVisible(bool _visible) { m_visible = _visible; }            // �\���t���O�̃Z�b�^�[
	bool GetVisible() const { return m_visible; }                           // �\���t���O�̃Q�b�^�[

	bool GetIsSkeletal() const { return m_isSkeletal; }                     // �X�P���^�����ǂ����̃Q�b�^�[

	void SetIntensityVal(float _val) { m_intensity = _val; }            // �P�x���x�̃Z�b�^�[

	void SetDrawMap(bool _flg) { m_isDrawMap = _flg; }
	void SetMapColor(const Vector3& _color) { m_mapColor = _color; }    // �}�b�v�`��p�̃J���[�Z�b�^�[

protected:

	class Mesh* m_mesh;                                                     // ���b�V���I�u�W�F�N�g�̊i�[�p
	size_t m_textureIndex;                                                  // �e�N�X�`���[�̃C���f�b�N�X

	bool m_visible;                                                         // �\���t���O
	bool m_isDrawMap;                                                       // �}�b�v��ʂɕ\�����邩
	bool m_isSkeletal;                                                      // �X�P���^�����f�����ǂ���

	float m_intensity;                                                      // �P�x�̋��x��� (�G�~�b�V�u�}�b�v��K�p���Ă��郁�b�V���̂�)

	Vector3 m_mapColor;                                                     // �}�b�v�`�掞�̃J���[���
};