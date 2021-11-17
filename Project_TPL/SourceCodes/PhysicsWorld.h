#pragma once
#include "GameMain.h"
#include "Tag.h"
#include <unordered_map>
#include <vector>

class PhysicsWorld
{
public:


	struct ColliderPairs
	{
		OBJECT_TAG pair1;
		OBJECT_TAG pair2;
	};


	PhysicsWorld();
	~PhysicsWorld();

	void DebugVisualizeCollisions(class GLSLprogram* _shader);
	void DrawCollisions(class GLSLprogram* _shader, const std::vector<class ColliderComponent*>& _colliderComps, const Vector3& _visualColor);

	// �����蔻��ǉ�/�폜�֐�
	void AddColliderComponent(class ColliderComponent* _collComp);
	void RemoveColliderComponent(class ColliderComponent* _collComp);

	void SetOneSideReactionColliderPair(OBJECT_TAG _noReactType, OBJECT_TAG _reactType);
	void SetDualReactionColliderPair(OBJECT_TAG _react1, OBJECT_TAG _react2);
	void SetSelfReactionCollider(OBJECT_TAG _selfReact);


	void UpdateCollision();                                                         // �R���W����
	
	void DebugShowBox();                                                      // �f�o�b�O�p�{�b�N�X�\��(������)
	void ToggleDebugMode() { m_boolDebugMode = !m_boolDebugMode; }            // �f�o�b�O���[�h

private:

	void OneReactionMatch(ColliderPairs _pair);
	void DualReactionMatch(ColliderPairs _pair);
	void SelfReactionMatch(OBJECT_TAG _tag);



	void CreateLineColors();





	// �����蔻��R���|�[�l���g�i�[�p
	std::unordered_map<OBJECT_TAG, std::vector<class ColliderComponent*>> m_colliderComps;

	std::vector<ColliderPairs> m_oneSideReactions;        // �Е��̂݃��A�N�V�������s�������蔻��y�A�̃��X�g
	std::vector<ColliderPairs> m_dualReactions;           // �o�����Ń��A�N�V�������s�������蔻��y�A�̃��X�g
	std::vector<OBJECT_TAG> m_selfReactions;              // ���O���[�v���ł̏������s�������蔻�胊�X�g

	std::vector<Vector3> m_lineColors;                    // �����蔻��{�b�N�X�̃J���[(Debug���Ɏg�p)

	bool m_boolDebugMode;

	void PlayerAndBGTest();                                                   // �v���C���[�ƕǂ̓����蔻��e�X�g

};