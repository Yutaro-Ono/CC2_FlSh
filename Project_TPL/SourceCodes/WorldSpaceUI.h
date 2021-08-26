//------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X(���[���h��ԗp��UI)
//
//------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <string>

class WorldSpaceUI
{

public:

	WorldSpaceUI(const Vector3& _pos, const std::string& _filePath, float _scale = 1.0f);
	WorldSpaceUI(const Vector3& _pos, class Texture* _texture, float iscale = 1.0f);
	~WorldSpaceUI();


	void SetPosition(const Vector3& _pos) { m_position = _pos; }
	void SetTexture(class Texture* _tex) { m_texture = _tex; }
	int GetTextureID() { return m_textureID; }

	void Draw(class GLSLprogram* _shader);

	// ���W�̃Z�b�g
	void SetUIPosition(const Vector3& _pos);

	// �r���{�[�h�s����`�E�擾
	Matrix4 GetBillboardMatrix();

	// �\����\���t���O�̃Z�b�^�[
	void SetVisible(bool _visible) { m_isVisible = _visible; }

	void SetScale(float _scale) { m_scale = _scale; }

private:


	Vector3 m_position;                      // ���W

	float m_scale;                           // �g�嗦

	class Texture* m_texture;                // �e�N�X�`��
	int m_textureID;                         // �e�N�X�`��ID

	bool m_isVisible;                        // �\��(false), ��\��(true)�t���O


	Matrix4 m_worldTransform;                // ���[���h�ϊ��s��
	
	static Matrix4 m_staticBillboardMat;     // �r���{�[�h�s�� (�J�����ƕ��s�ɂȂ�悤�ɔz�u���邽�߂̍s��)


};