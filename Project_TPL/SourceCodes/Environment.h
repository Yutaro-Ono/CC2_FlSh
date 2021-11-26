#pragma once
#include "Renderer.h"

// ���^�C�v
enum class ENVIRONMENT_TYPE : unsigned char
{
	MORNING,
	AFTERNOON,
	EVENING,
	NIGHT,
	NIGHT_CITY,
	SPACE,
	TITLE
};

class Environment
{
public:

	Environment(class Actor* _target, ENVIRONMENT_TYPE _envtype);                                // �R���X�g���N�^2
	~Environment();                         // �f�X�g���N�^

	void Update();

	void SetSkyBox(class SkyBox* _skybox);
	void SetDirectionalLight(ENVIRONMENT_TYPE _envType, const Vector3& _pos);             // ���ԑт��Ƃɕ��s���C�g��ݒ肷��

	void SetDirectionalLightPos(const Vector3& _pos);

	// ���^�C�v�̃Q�b�^�[�E�Z�b�^�[
	ENVIRONMENT_TYPE GetEnvironmentType() { return m_envType; }
	void SetEnvironmentType(ENVIRONMENT_TYPE _envType) { m_envType = _envType; }
	// �X�J�C�{�b�N�X�N���X�̃Q�b�^�[
	class SkyBox* GetSkyBox() { return m_skyBox; }

private:


	ENVIRONMENT_TYPE m_envType;

	class Actor* m_lightTarget;

	DirectionalLight* m_dirLight;

	// �X�J�C�{�b�N�X
	class SkyBox* m_skyBox;
};