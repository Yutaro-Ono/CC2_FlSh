#pragma once
#include "Component.h"

class CarMeshComponent : public Component
{

public:

	CarMeshComponent(class Actor* _owner);
	~CarMeshComponent();


	virtual void Draw(class GLSLprogram* _shader);                             // ���b�V���R���|�[�l���g�̕`��
	virtual void DrawShadow(class GLSLprogram* _shader);                       // �V���h�E�����p�̕`��

	virtual void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }         // ���b�V���R���|�[�l���g�Ŏg�p���郁�b�V���̃Z�b�g

	void SetTextureIndex(size_t _index) { m_textureIndex = _index; }    // �e�N�X�`���̃Z�b�g

	void SetReflect(bool _ref) { m_reflect = _ref; }                    // ���ˌv�Z�̗L��

	void SetVisible(bool _visible) { m_visible = _visible; }            // �\���t���O�̃Z�b�^�[
	bool GetVisible() const { return m_visible; }                           // �\���t���O�̃Q�b�^�[



protected:

	class Mesh* m_mesh;                                                     // ���b�V���I�u�W�F�N�g�̊i�[�p
	size_t m_textureIndex;                                                  // �e�N�X�`���[�̃C���f�b�N�X

	bool m_reflect;                                                         // ���ˌv�Z���s����

	bool m_visible;                                                         // �\���t���O
};