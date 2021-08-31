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

	// �z��ւ̒ǉ����� (�˗��l�E����n�_)
	void AddClientActor(class ClientActor* in_client);
	void AddPatrolPoint(class PatrolPoint* in_patrolPoint);

	class PlayerManager* GetPlayer() { return m_player; }

	// ����n�_�E�˗��l�A�N�^�̃V���b�t������
	void ShufflePatrolPoint();
	void ShuffleClientActor();
	
	// �˗��l�A�N�^�̔z��Q�b�^�[
	const std::vector<class ClientActor*>& GetClients() { return m_clients; }

	// ����n�_�̔z��Q�b�^�[
	const std::vector<class PatrolPoint*>& GetPatrolPoint() { return m_patrolPoints; }

	// �w���z��̃Q�b�^�[
	const std::vector<class Helicopter*>& GetHeliArray() { return m_helicopters; }

	void ImGuiDebugWorld();

	// �n�`�}�l�[�W���̃|�C���^�Q�b�^�[
	class LevelManager* GetLevelManager() { return m_level; }

	// UI�L�����o�X�̃Q�b�^�[
	class CanvasGameScene* GetCanvas() { return m_canvas; }

	bool GetFoundPlayer() { return m_foundPlayer; }

private:

	std::vector<class ClientActor*> m_clients;          // �˗��҃A�N�^�[
	std::vector<class PatrolPoint*> m_patrolPoints;     // ����n�_�̔z��
	std::vector<class Helicopter*> m_helicopters;        // �w���R�v�^�[�̔z��

	class PlayerManager* m_player;

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class CanvasGameScene* m_canvas;                     // UI�L�����o�X

	class MiniMapHUD* m_mapHUD;                 // �}�b�vHUD

	class Environment* m_environment;
	ENVIRONMENT_TYPE m_envType;


	// �t���O�Ǘ�
	bool m_foundPlayer;     // (��@�ł�)�w�����v���C���[����������
};