#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Actor.h"
#include "PlayerCar.h"
#include "PlayerHuman.h"
#include "ThirdPersonCamera.h"
#include "Collision.h"
#include "ColliderComponent.h"
#include "BoxColliderComponent.h"
#include "WallColliderComponent.h"
#include "LineColliderComponent.h"
#include "GLSLprogram.h"

// �R���X�g���N�^
PhysicsWorld::PhysicsWorld()
	:m_boolDebugMode(false)
{
	std::cout << "CREATE : PhysicsWorld class Instance" << std::endl;

	CreateLineColors();


}

PhysicsWorld::~PhysicsWorld()
{
	std::cout << "DELETE : PhysicsWorld class Instance" << std::endl;
	//m_bgBoxes.clear();
}

/// <summary>
/// �����蔻��̉���
/// </summary>
void PhysicsWorld::DebugVisualizeCollisions(GLSLprogram* _shader)
{
	if (!m_boolDebugMode)
	{
		return;
	}

	// �����蔻��{�b�N�X�`�� tag���ɐF��ς��Ă��ׂẴ��X�g�\��
	int colorCount = 0;
	size_t colorNum = m_lineColors.size();
	for (auto t = OBJECT_TAG::BEGIN; t != OBJECT_TAG::END; ++t)
	{
		DrawCollisions(_shader, m_colliderComps[t], m_lineColors[colorCount % colorNum]);
		colorCount++;
	}
}

/// <summary>
/// �����蔻��̕`�揈��
/// </summary>
/// <param name="_colliderComps"> �`�悷�铖���蔻�� </param>
/// <param name="_visualColor"> �`��J���[ </param>
void PhysicsWorld::DrawCollisions(GLSLprogram* _shader, const std::vector<class ColliderComponent*>& _colliderComps, const Vector3& _visualColor)
{
	// �v���W�F�N�V���� * �r���[ �����s��
	Matrix4 view, proj, projView;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	projView = proj * view;

	// ���[���h�ϊ��s��쐬�p
	Matrix4 scaleMat, posMat, rotMat, worldMat, slopeRot;
	Vector3 scale, pos;

	// �`��p�V�F�[�_�[�̗L����
	_shader->UseProgram();
	_shader->SetUniform("u_projView", projView);
	_shader->SetUniform("u_color", _visualColor);

	for (auto item : _colliderComps)
	{
		// Box�������ꍇ�̕`��
		if (item->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			AABB box;
			Vector3 min, max;
			box = dynamic_cast<BoxColliderComponent*>(item)->GetWorldBox();

			// �{�b�N�X�̃X�P�[���ƈʒu���擾
			min = box.m_min;
			max = box.m_max;
			scale = max - min;
			pos = min;

			scaleMat = Matrix4::CreateScale(scale);
			posMat = Matrix4::CreateTranslation(pos);

			worldMat = scaleMat * posMat;
			_shader->SetUniform("u_worldTransform", worldMat);

			RENDERER->SetActiveBoxVAO();
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		}
		// Wall�������ꍇ�̕`�� 
		if (item->GetColliderType() == COLLIDER_TYPE::TYPE_WALL||
			item->GetColliderType() == COLLIDER_TYPE::TYPE_SLOPE)
		{
			WallColliderComponent* wallcol;
			Wall walldata;

			Vector3 scale; // �`��X�P�[�����O�W��
			Vector3 pos; // �`��ʒu
			Vector3 normal; // �ǖ@��

			// WallCollider�ƕǃf�[�^�擾
			wallcol = dynamic_cast<WallColliderComponent*>(item);
			walldata = wallcol->GetWall();
			normal = walldata.m_normal;
			// 4�_�̒��_�����߂�
			for (int i = 0; i < 4; i++)
			{
				pos += walldata.m_wallVertices[i];
			}
			pos = pos * 0.25f;

			// �s��
			scaleMat = Matrix4::CreateScale(walldata.m_scale);
			rotMat = Matrix4::CreateRotationZ(walldata.m_zRotate);
			posMat = Matrix4::CreateTranslation(pos);
			slopeRot = Matrix4::CreateRotationY(walldata.m_slopeAngle);

			worldMat = scaleMat * slopeRot * rotMat * posMat;
			_shader->SetUniform("u_worldTransform", worldMat);

			RENDERER->SetActiveSquareVAO();
			glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, 0);
		}
		// LINE�������ꍇ
		if (item->GetColliderType() == COLLIDER_TYPE::TYPE_LINE)
		{
			LineColliderComponent* linecol;
			Line line;

			linecol = dynamic_cast<LineColliderComponent*>(item);

			Vector3 scale;  // �`��X�P�[�����O�W��
			Vector3 pos;    // �`��ʒu
			line = linecol->GetLine();

			// �����x�N�g���Ɛ��K�����������x�N�g�����߂�
			Vector3 lineVec = line.m_endPoint - line.m_startPoint;
			Vector3 lineDir = lineVec;
			lineDir.Normalize();

			//����������X�P�[�����O�s��쐬
			float len = lineVec.Length();
			Matrix4 scaleMat = Matrix4::CreateScale(len, 0, 0);

			// ���������]���Ɖ�]�p�����߃N�I�[�^�j�I���쐬
			Vector3 rotAxis;
			rotAxis = Vector3::Cross(lineDir, Vector3(1, 0, 0));
			float rotAngle = -1.0f * acosf(Vector3::Dot(rotAxis, lineDir));

			// �N�I�[�^�j�I����������̕����ɉ�]����s����쐬
			Quaternion q(rotAxis, rotAngle);
			Matrix4 rotMat = Matrix4::CreateFromQuaternion(q);

			// ���s�ړ�����
			Matrix4 posMat = Matrix4::CreateTranslation(line.m_startPoint);


			worldMat = scaleMat * rotMat * posMat;
			_shader->SetUniform("u_worldTransform", worldMat);

			//�`��
			RENDERER->SetActiveLineVAO();
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		}
	}
}


/// <summary>
/// �����蔻��R���|�[�l���g�̒ǉ�����
/// </summary>
/// <param name="_collComp"> �ǉ����铖���蔻��R���|�[�l���g </param>
void PhysicsWorld::AddColliderComponent(ColliderComponent* _collComp)
{
	// �w�肵���^�O�ԖڂɃR���|�[�l���g��ǉ�
	OBJECT_TAG tag = _collComp->GetOwnerTag();
	m_colliderComps[tag].emplace_back(_collComp);
}

/// <summary>
/// �w�肵�������蔻��R���|�[�l���g�̍폜
/// </summary>
/// <param name="_collComp"> �폜�����������蔻��R���|�[�l���g </param>
void PhysicsWorld::RemoveColliderComponent(ColliderComponent* _collComp)
{
	OBJECT_TAG tag = _collComp->GetOwnerTag();
	// �^�O�ɂ���či�荞�񂾘A�z�z������������Ď擾
	std::vector<ColliderComponent*>::iterator itr = std::find(m_colliderComps[tag].begin(), m_colliderComps[tag].end(), _collComp);
	// �z����ɑ��݂��Ă����ꍇ�A�R���|�[�l���g������
	if (itr != m_colliderComps[tag].end())
	{
		m_colliderComps[tag].erase(itr);
		return;
	}
}

/// <summary>
/// �Е��̂݃��A�N�V�������铖���蔻��y�A�̓o�^
/// </summary>
/// <param name="_noReactType"> ���A�N�V������Ԃ��Ȃ����̃^�O </param>
/// <param name="_reactType"> �����Е�����̉e�����󂯂���̃^�O </param>
void PhysicsWorld::SetOneSideReactionColliderPair(OBJECT_TAG _noReactType, OBJECT_TAG _reactType)
{
	ColliderPairs pair;

	pair.pair1 = _noReactType;
	pair.pair2 = _reactType;

	m_oneSideReactions.emplace_back(pair);
}

/// <summary>
/// �o�����Ń��A�N�V�������铖���蔻��y�A�̓o�^
/// </summary>
/// <param name="_react1"> �����蔻��1�̃^�O </param>
/// <param name="_react2"> �����蔻��2�̃^�O </param>
void PhysicsWorld::SetDualReactionColliderPair(OBJECT_TAG _react1, OBJECT_TAG _react2)
{
	ColliderPairs pair;

	pair.pair1 = _react1;
	pair.pair2 = _react2;

	m_dualReactions.emplace_back(pair);
}

/// <summary>
/// ���O���[�v���œ����蔻�菈������^�O�̓o�^
/// </summary>
/// <param name="_selfReact"> �����蔻�肷��^�O </param>
void PhysicsWorld::SetSelfReactionCollider(OBJECT_TAG _selfReact)
{
	m_selfReactions.emplace_back(_selfReact);
}


/// <summary>
/// �����蔻��̍X�V��������
/// </summary>
void PhysicsWorld::UpdateCollision()
{
	// �Е��̂݃��A�N�V�������铖���蔻��
	for (auto oneReactPair : m_oneSideReactions)
	{
		OneReactionMatch(oneReactPair);
	}

	// �o�������A�N�V�������铖���蔻��
	for (auto twoSideReactPair : m_dualReactions)
	{
		DualReactionMatch(twoSideReactPair);
	}

	// �������X�g�̓����蔻��
	for (auto selfReact : m_selfReactions)
	{
		SelfReactionMatch(selfReact);
	}

}

void PhysicsWorld::DebugShowBox()
{
}

/// <summary>
/// �Е��̂݃��A�N�V�����̓����蔻��y�A�̏Փ˔��菈��
/// </summary>
/// <param name="_pair"> �Е����A�N�V�����y�A </param>
void PhysicsWorld::OneReactionMatch(ColliderPairs _pair)
{
	for (auto noreactColl : m_colliderComps[_pair.pair1])
	{
		for (auto reactColl : m_colliderComps[_pair.pair2])
		{
			if (noreactColl->CollisionDetection(reactColl))
			{
				reactColl->GetOwner()->OnCollisionEnter(reactColl, noreactColl);
			}
		}
	}
}

/// <summary>
/// �o�������A�N�V�����̓����蔻��y�A�̏Փ˔��菈��
/// </summary>
/// <param name="_pair"> �o�������A�N�V�����y�A </param>
void PhysicsWorld::DualReactionMatch(ColliderPairs _pair)
{
	for (auto coll1 : m_colliderComps[_pair.pair1])
	{
		for (auto coll2 : m_colliderComps[_pair.pair2])
		{
			if (coll1->CollisionDetection(coll2))
			{
				coll1->GetOwner()->OnCollisionEnter(coll1, coll2);
			}
		}
	}
}

/// <summary>
/// ���O���[�v���̏Փ˔��菈��
/// </summary>
/// <param name="_tag"> ���菈�����s���O���[�v�^�O </param>
void PhysicsWorld::SelfReactionMatch(OBJECT_TAG _tag)
{
	// ���^�O�̓����蔻��̓o�^����ێ�
	size_t size = m_colliderComps[_tag].size();

	for (int i = 0; i < size; i++)
	{
		// �����ȊO�̓����蔻��𑍓�����Ń`�F�b�N
		for (int j = i + 1; j < size; i++)
		{
			if (m_colliderComps[_tag][i]->CollisionDetection(m_colliderComps[_tag][j]))
			{
				Actor* actor1 = m_colliderComps[_tag][i]->GetOwner();
				actor1->OnCollisionEnter(m_colliderComps[_tag][i], m_colliderComps[_tag][j]);
			}
		}
	}
}




/// <summary>
/// �f�o�b�O�p�����蔻��{�b�N�X�J���[�̃Z�b�g
/// </summary>
void PhysicsWorld::CreateLineColors()
{
	m_lineColors.emplace_back(Vector3(1.0f, 1.0f, 1.0f));
	m_lineColors.emplace_back(Vector3(1.0f, 0.0f, 0.0f));       // RED
	m_lineColors.emplace_back(Vector3(0.0f, 1.0f, 0.0f));       // GREEN
	m_lineColors.emplace_back(Vector3(0.0f, 0.0f, 1.0f));       // BLUE
	m_lineColors.emplace_back(Vector3(1.0f, 1.0f, 0.0f));       // YELLOW
	m_lineColors.emplace_back(Vector3(1.0f, 0.71f, 0.76f));     // PINK
	m_lineColors.emplace_back(Vector3(0.56f, 0.93f, 0.56f));    // LIGHT GREEN
	m_lineColors.emplace_back(Vector3(1.0f, 1.0f, 0.88f));      // LIGHT YELLOW
	m_lineColors.emplace_back(Vector3(0.68f, 0.85f, 0.9f));     // LIGHT BLUE
}