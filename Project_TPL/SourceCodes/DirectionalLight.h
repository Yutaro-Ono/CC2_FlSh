//----------------------------------------------------------------------------------+
// @file        DirectionalLight.h
// @brief       �f�B���N�V���i�����C�g�N���X
// @note        �����_���[�ɓo�^���邱�ƂŁA�`�掞�Ƀ��C�e�B���O�����f�����
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Math.h"



class DirectionalLight
{

public:

	DirectionalLight();
	~DirectionalLight();

	void ReAdjustmentDirection();

	// Setter/Getter
	void SetTarget(const Vector3& _targetPos) { m_targetPos = _targetPos; m_readjustDir = true; }
	void SetLightDirection(const Vector3& _lightDir) { m_direction = _lightDir; }
	void SetDiffuse(const Vector3& _color) { m_diffuse = _color; }
	void SetSpecular(const Vector3& _color) { m_specular = _color; }
	void SetAmbient(const Vector3& _color) { m_ambient = _color; }
	void SetIntensity(const float& _intens) { m_intensity = _intens; }

	const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetTargetPos() const { return m_targetPos; }
	const Vector3& GetDirection() const { return m_direction; }
	const Vector3& GetDiffuse() const { return m_diffuse; }
	const Vector3& GetSpecular() const { return m_specular; }
	const Vector3& GetAmbient() const { return m_ambient; }
	const float& GetIntensity() const { return m_intensity; }

private:

	Vector3 m_position;           // �����ʒu
	Vector3 m_targetPos;             // �^�[�Q�b�g����ʒu
	Vector3 m_direction;          // ���̌���

	Vector3 m_diffuse;            // �f�B�t���[�Y
	Vector3 m_specular;           // �X�y�L����
	Vector3 m_ambient;            // �A���r�G���g
	float m_intensity;            // ���̕��o����G�l���M�[��

	bool m_readjustDir;           // ���̕����̍Ē����t���O

	// �f�o�b�O�p
	class DirectionalLightDebug* m_debugObj;

	// friend�֌W
	friend class DirectionalLightDebug;
	friend class Renderer;

};
