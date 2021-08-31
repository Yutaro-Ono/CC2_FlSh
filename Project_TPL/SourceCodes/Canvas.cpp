#include "Canvas.h"
#include "UIScreen.h"
#include "WorldBase.h"
#include "PlayerManager.h"
#include "PlayerControlUI.h"
#include "ScoreUI.h"

// �R���X�g���N�^
Canvas::Canvas(WorldBase* in_world)
	:m_world(in_world)
{
}

// �f�X�g���N�^
Canvas::~Canvas()
{
	// �S�Ă�UI���I��
	for (auto ui : m_interfaces)
	{
		ui->Close();
	}
	m_interfaces.clear();
}

/// <summary>
/// �C���^�[�t�F�[�X�̍X�V����
/// </summary>
/// <param name="_deltaTime"> �f���^�^�C�� </param>
void Canvas::Update(float _deltaTime)
{
	for (auto ui : m_interfaces)
	{
		ui->Update(_deltaTime);
	}
}
