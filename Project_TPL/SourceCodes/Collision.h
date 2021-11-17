//----------------------------------------------------------------------------------+
// @file        Collision.h
// @brief       �����蔻��N���X
// @note        �e�����蔻��ɕK�v�ȏ��Ə������`
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2019         �V�K�쐬
// 2021/11/17   CollisionInfo�\���̂��`
//----------------------------------------------------------------------------------+
#pragma once
#include "GameMain.h"


//----------------------------------------------------+
// �Փˏ�񑍍�
//----------------------------------------------------+
typedef struct CollisionInfo
{
	Vector3 m_collisionPoint;       // �Փ˓_
	Vector3 m_fixVec;               // �����蔻��ɂ��C���x�N�g��(�����߂�)
	Vector3 m_hitNormal;            // �Փ˓_�ɂ�����@��
};


//----------------------------------------------------+
// �����̓����蔻��
//----------------------------------------------------+
typedef struct Line
{
	Line();
	Line(const Vector3& _startPoint, const Vector3& _endPoint);

	Vector3 m_startPoint;        // �����̎n�_
	Vector3 m_endPoint;          // �����̏I�_

	Vector3 m_normalizedDir;     // ���K�����������x�N�g��
	float m_lineLength;          // �����̒���
	bool m_rotatable;            // ��]�̉�
};


//----------------------------------------------------+
// ��(�X���[�v��)�̓����蔻��
//----------------------------------------------------+
typedef struct Wall
{
	// �ǂ̊p�x��`�p��
	enum WALL_ANGLE
	{
		ANGLE_0 = 0,
		ANGLE_90 = 90,
		ANGLE_180 = 180,
		ANGLE_270 = 270
	};

	Wall();
	bool Contains(const Vector3& _pos);


	static Wall CreateWall(const Vector3& _start, const Vector3& _end, float _height);
	static Wall CreateSlope(const Vector3& _pos, Wall::WALL_ANGLE _unitZDegree, float _horizonLength, float _height, float _width);


	void CalcAABB();


	AABB m_wallAABB;                // �ǑS�̂�AABB
	Vector3 m_normal;               // �ǖʂ̖@��
	Vector3 m_startPoint;           // �ǂ̊J�n�_
	Vector3 m_endPoint;             // �ǂ̏I���_
	Vector3 m_wallVertices[4];          // �ǂ��\������4���_�̍��W
	Vector3 m_scale;                // �`�掞�g�p(x, y, z�X�P�[��)

	float m_planeD;                 // �ǖʂ̕��ʕ������ɗp����D�l
	float m_zRotate;                // Z����]�l
	float m_slopeAngle;             // �X���[�v���̌X�Ίp

	bool m_isSlope;                 // �X���[�v���ǂ���
};

//----------------------------------------------------+
// ����
//----------------------------------------------------+
typedef struct Sphere
{

	Sphere(const Vector3& _center, float _radius);                                      // �R���X�g���N�^
	bool Contains(const Vector3& _point) const;                                           // �|�C���g(���W)�����̓��Ɋ܂܂�Ă��邩
	bool ContainsSphere(const Sphere _otherSphere) const;                                 // ����̋��̂Ƃ��̋��̂��ڐG���Ă��邩

	Vector3      m_center;                                                                  // ���̂̒��S���W
	float        m_radius;                                                                  // ���̂̔��a

}Sphere;


//----------------------------------------------------+
// ����
//----------------------------------------------------+
typedef struct Plane
{

	Plane(const Vector3& _normal, const Vector3 _position);                             // �R���X�g���N�^

	Vector3      m_normal;                                                                  // �@��
	Vector3      m_position;                                                                // ���ʏ�̓_
	
}Plane;


//----------------------------------------------------+
// �����s�{�b�N�X (AABB:Axis-Aligned Bounding Box)
//----------------------------------------------------+
typedef struct AABB
{

	AABB();                                                                                 // �R���X�g���N�^
	AABB(const Vector3& _min, const Vector3& _max);                                     // �������p�R���X�g���N�^

	
	void         InitMinMax(const Vector3& _point) { m_min = m_max = _point; }          // ���f���ǂݍ��ݎ��̍ŏ��ő�̓_�����߂�
	void         SetBoxVertex();                                                          // Box���\������8���_�̃Z�b�g
	
	void         UpdateMinMax(const Vector3& _point);                                     // �ŏ��ő�̓_���X�V

	bool         Contains(const Vector3& _point) const;                                   // �_���{�b�N�X���Ɋ܂܂�邩
	float        MinDistSq(const Vector3& _point) const;                                  // �_�Ƃ̍ŏ����������߂�

	void         Rotate(const Quaternion& _quaternion);                                   // ��]
	void         SetAllowRotate(bool _value) { m_isRotatable = _value; }                  // ��]�������邩

	void         SetScale(float _scaleX, float _scaleY, float _scaleZ);                   // AABB�̃X�P�[�����O

	// ���ڒ��_���W���Z�b�g
	void         SetMinVector(const Vector3& _minVec) { m_min = _minVec; }
	void         SetMaxVector(const Vector3& _maxVec) { m_max = _maxVec; }

	Vector3      m_min;                                                                     // �ŏ��l
	Vector3      m_max;                                                                     // �ő�l
	Vector3      m_vertices[8];                                                             // Box���\������8���_

	bool         m_isRotatable  =  false;                                                   // ��]���邩�ǂ���

}AABB;


//----------------------------------------------------+
// �L���{�b�N�X (OBB:Oriented Bounding Box)
//----------------------------------------------------+
typedef struct OBB
{

	OBB();                                                                                  // �R���X�g���N�^
	OBB(const Vector3& _position, const Vector3& _degree, const Vector3& _scale);     // �������p�R���X�g���N�^
	OBB(AABB _aabb);                                                                      // 

	Vector3     GetDirection(int _element);                                               // 
	Vector3     GetPositionWorld();                                                         // 

	float       GetLength(int _element);                                                  // 
	Matrix4     GetMatrix();                                                                // 
	void        Transform(Matrix4& _matrix);                                              // 

	Vector3     m_position;                                                                 // 
	Vector3     m_normalDirection[3];                                                       // 
	float       m_length[3];                                                                // 

	AABB        m_wrapAABB;                                                                 // OBB�����͂�AABB

}OBB;


bool Intersect(const AABB& _boxA, const AABB& _boxB);                                         // AABB���m�̏Փ�
bool Intersect(const Sphere& _sphere, const AABB& _box);                                // ���̂�AABB�Ƃ̏Փ�
bool Intersect(const AABB& _box, const Wall& _wall, CollisionInfo& _info_);                 // AABB�ƕǖʂ̏Փ�
bool Intersect(const AABB& _box, const Vector3& _point, CollisionInfo& _info_);             // AABB�Ɠ_�̏Փ�
bool Intersect(const Line& _line, const AABB& _box, CollisionInfo& _info_);                 // ������AABB�̏Փ�
bool Intersect(const Wall& _wall, const Line& _line, CollisionInfo& _info_);                 // �ǖʂƐ����̓����蔻��

bool IntersectWallEdgeFix(const AABB& _box, const Vector3& _edgeLow, const Vector3& _edgeHi, Vector3& _fixVec_);

// AABB�̂߂荞�݂�߂��ʂ��v�Z
void CalcCollisionFixVec(const AABB& _movableBox, const AABB& _fixedBox, Vector3& _calcFixVec_);
// �_�ƕ��ʂƂ̕����t��������Ԃ�
float PlaneToPointDist(const Vector3& _position, const Plane _plane);