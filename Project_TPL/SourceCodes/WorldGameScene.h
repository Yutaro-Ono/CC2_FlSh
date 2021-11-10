//---------------------------------------------------------+
//
// �Q�[�����[���h�����N���X
//
//---------------------------------------------------------+
#pragma once
#include <vector>
#include "WorldBase.h"
#include "Environment.h"

class WorldGameScene : public WorldBase
{

public:


	WorldGameScene();             // �R���X�g���N�^
	~WorldGameScene();            // �f�X�g���N�^

	bool Load() override;

	void Update(float in_deltaTime);
	void UpdateWorld(float _deltaTime) override;

	void ImGuiDebugWorld();

	// �v���C���[�̃Q�b�^�[
	class Player* GetPlayer() { return m_player; }
	// �n�`�}�l�[�W���̃|�C���^�Q�b�^�[
	class LevelManager* GetLevelManager() { return m_level; }

private:

	class Player* m_player;
	class LevelManager* m_level;

};