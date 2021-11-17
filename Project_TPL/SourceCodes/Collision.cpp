//-----------------------------------------------------------------------+
// �Փˌ��m(�����蔻��)�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Collision.h"
#include <algorithm>
#include <array>

float LensSegOnSeparateAxis(Vector3& _separate, Vector3& _e1, Vector3& _e2, Vector3& _e3);     // �������ɓ��e���ꂽ���������瓊�e���������Z�o
float LensSegOnSeparateAxis(Vector3& _separate, Vector3& _e1, Vector3& _e2);                     // �������ɓ��e���ꂽ���������瓊�e���������Z�o

//-------------------------------------------------------------+
// ����
//-------------------------------------------------------------+
// �R���X�g���N�^
// (mCenter:������, mRdius:������)
Sphere::Sphere(const Vector3 & _center, float _radius)
	:m_center(_center)
	,m_radius(_radius)
{
	// �����Ȃ�
}

// ���̓����蔻�菈��(Sphere�̒���point���܂܂�Ă��邩)
bool Sphere::Contains(const Vector3 & _point) const
{
	// ���̒��S�Ɠ_�Ƃ̋����̓��
	float distSq = (m_center - _point).LengthSq();
	// ���Ɣ��a��2����r���A�Ԃ�
	return distSq <= m_radius * m_radius;
}

// ���̓��m�̓����蔻��
bool Sphere::ContainsSphere(const Sphere _otherSphere) const
{
	// ���̂Ƌ��̂̒��S�_�̋����̓����擾
	float distSq = (m_center - _otherSphere.m_center).LengthSq();
	// ���̓��m�̔��a�̓�悩���r���A�~�Ɖ~���G��Ă��邩��Ԃ�
	return distSq <= (m_radius + _otherSphere.m_radius) * (m_radius + _otherSphere.m_radius);
}


//-------------------------------------------------------------+
// ����
//-------------------------------------------------------------+
// �R���X�g���N�^
// (mNormal:������, mPos:������
Plane::Plane(const Vector3 & _normal, const Vector3 _position)
	:m_normal(_normal)
	,m_position(_position)
{
	// �����Ȃ�
}


//-------------------------------------------------------------+
// �����s�{�b�N�X (AABB:Axis-Aligned Bounding Box)
//-------------------------------------------------------------+
// �R���X�g���N�^
AABB::AABB()
{
	m_min = Vector3(0.0f, 0.0f, 0.0f);
	m_max = Vector3(0.0f, 0.0f, 0.0f);
}

// �������p�R���X�g���N�^
AABB::AABB(const Vector3 & _min, const Vector3 & _max)
	:m_min(_min)
	,m_max(_max)
{
	// ���_�̃Z�b�g
	SetBoxVertex();
}

/// <summary>
/// Box���\������8���_�̃Z�b�g
/// </summary>
void AABB::SetBoxVertex()
{
	// ���̏㑤4���_
	m_vertices[0] = Vector3(m_min.x, m_min.y, m_max.z);
	m_vertices[1] = Vector3(m_max.x, m_min.y, m_max.z);
	m_vertices[2] = Vector3(m_min.x, m_max.y, m_max.z);
	m_vertices[3] = Vector3(m_max.x, m_max.y, m_max.z);

	// ���̉���4���_
	m_vertices[4] = Vector3(m_min.x, m_min.y, m_min.z);
	m_vertices[5] = Vector3(m_max.x, m_min.y, m_min.z);
	m_vertices[6] = Vector3(m_min.x, m_max.y, m_min.z);
	m_vertices[7] = Vector3(m_max.x, m_max.y, m_min.z);
}

// �ŏ��l�ƍő�l���X�V
void AABB::UpdateMinMax(const Vector3 & _point)
{

	// x,y,z���ꂼ��̍ŏ��l�����߂�
	m_min.x = Math::Min(m_min.x, _point.x);
	m_min.y = Math::Min(m_min.y, _point.y);
	m_min.z = Math::Min(m_min.z, _point.z);

	// x,y,z���ꂼ��̍ő�l�����߂�
	m_max.x = Math::Max(m_max.x, _point.x);
	m_max.y = Math::Max(m_max.y, _point.y);
	m_max.z = Math::Max(m_max.z, _point.z);

}

/// <summary>
/// Box���Ɏw�肵���_�������Ă��邩
/// </summary>
/// <param name="_point"> ���ׂ���W�|�C���g </param>
/// <returns> �����ɓ_�����݂��邩 </returns>
bool AABB::Contains(const Vector3 & _point) const
{
	
	bool outside = _point.x < m_min.x ||
		_point.y < m_min.y ||
		_point.z < m_min.z ||
		_point.x > m_max.x ||
		_point.y > m_max.y ||
		_point.z > m_max.z;

	// �������true���Ȃ���΁Abox�̓����ɓ_�����݂���
	return !outside;
}

/// <summary>
/// �_��AABB�̋���(8���_���猩���ŏ�����)
/// </summary>
/// <param name="_point"></param>
/// <returns></returns>
float AABB::MinDistSq(const Vector3 & _point) const
{

	// ���ꂼ��̎��ł̍����Ƃ�
	float dx = Math::Max(m_min.x - _point.x, 0.0f);
	dx = Math::Max(dx, _point.x - m_max.x);
	float dy = Math::Max(m_min.y - _point.y, 0.0f);
	dy = Math::Max(dy, _point.y - m_max.y);
	float dz = Math::Max(m_min.z - _point.z, 0.0f);
	dz = Math::Max(dy, _point.z - m_max.z);

	// 3������Ԃł̋����̓��̌������
	return dx * dx + dy * dy + dz * dz;
}

/// <summary>
/// AABB�̉�]����
/// </summary>
/// <param name="_quaternion"> ��]�s�� </param>
void AABB::Rotate(const Quaternion & _quaternion)
{
	// �{�b�N�X��8�̒��_�̔z����
	std::array<Vector3, 8> points;
	// �ŏ��l�͏�ɃR�[�i�[�ł���
	points[0] = m_min;
	// 2�̍ŏ��l��1�̍ő�l�̕��בւ�
	points[1] = Vector3(m_max.x, m_min.y, m_min.z);
	points[2] = Vector3(m_min.x, m_max.y, m_min.z);
	points[3] = Vector3(m_min.x, m_min.y, m_max.z);
	// 2�̍ő�l��1�̍ŏ��l�̕��בւ�
	points[4] = Vector3(m_min.x, m_max.y, m_max.z);
	points[5] = Vector3(m_max.x, m_min.y, m_max.z);
	points[6] = Vector3(m_max.x, m_max.y, m_min.z);
	// �ő�l�͏�ɃR�[�i�[�ł���
	points[7] = Vector3(m_max);

	// �ŏ��̓_����]
	Vector3 p = Vector3::Transform(points[0], _quaternion);
	// �ŏ��l�ƍő�l�����߂邽�߂ɍŏ��̓_�����ă��Z�b�g
	m_min = p;
	m_max = p;
	// ��]�ɂ���čő�l�ƍŏ��l�����߂Ȃ����B
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], _quaternion);
		UpdateMinMax(p);
	}
}

/// <summary>
/// AABB�̃X�P�[�����O
/// </summary>
/// <param name="_scaleX"> X�g�嗦 </param>
/// <param name="_scaleY"> Y�g�嗦 </param>
/// <param name="_scaleZ"> Z�g�嗦 </param>
void AABB::SetScale(float _scaleX, float _scaleY, float _scaleZ)
{
	m_max = m_max * Vector3(_scaleX, _scaleY, _scaleZ);
	m_min = m_min * Vector3(_scaleX, _scaleY, _scaleZ);
}


//----------------------------------------------------+
// �L���{�b�N�X (OBB:Oriented Bounding Box)
//----------------------------------------------------+
// �R���X�g���N�^
OBB::OBB()
	:m_position(0.0f, 0.0f, 0.0f)
{
	for (int i = 0; i < 3; i++)
	{
		m_normalDirection[i] = Vector3::Zero;
		m_length[i] = 0.0f;
	}
}


OBB::OBB(const Vector3 & _position, const Vector3 & _degree, const Vector3 & _scale)
{

	Matrix4 matrix[3], finalMat;
	float theta[3];
	m_position = _position;

	// ��]�p(�x��)��胉�W�A���p�ɕϊ�
	theta[0] = Math::ToRadians(_degree.x);
	theta[1] = Math::ToRadians(_degree.y);
	theta[2] = Math::ToRadians(_degree.z);

	// ��]�p����]�s��쐬
	matrix[0] = Matrix4::CreateRotationX(theta[0]);
	matrix[1] = Matrix4::CreateRotationY(theta[1]);
	matrix[2] = Matrix4::CreateRotationZ(theta[2]);

	// ��]�s��̍��������x�N�g�����擾
	finalMat = matrix[0] * matrix[1] * matrix[2];

	m_normalDirection[0] = finalMat.GetXAxis();
	m_normalDirection[1] = finalMat.GetYAxis();
	m_normalDirection[2] = finalMat.GetZAxis();

	// �X�P�[��(�{�b�N�X����)�̔�����ۑ�
	m_length[0] = _scale.x * 0.5f;
	m_length[1] = _scale.y * 0.5f;
	m_length[2] = _scale.z * 0.5f;

}

// AABB�������l�ɂ���OBB�����
OBB::OBB(AABB _aabb)
{

	// ��]�Ȃ�
	m_normalDirection[0] = Vector3::UnitX;
	m_normalDirection[1] = Vector3::UnitY;
	m_normalDirection[2] = Vector3::UnitZ;

	// �e���̒���
	m_length[0] = (_aabb.m_max.x - _aabb.m_min.x) * 0.5f;
	m_length[1] = (_aabb.m_max.y - _aabb.m_min.y) * 0.5f;
	m_length[2] = (_aabb.m_max.z - _aabb.m_min.z) * 0.5f;

	// �{�b�N�X���S�̃��[���h���W
	m_position.x = _aabb.m_min.x + m_length[0];
	m_position.y = _aabb.m_min.y + m_length[1];
	m_position.z = _aabb.m_min.z + m_length[2];

}

// �e���̌������擾 0 = x��, 1 = y��, 2 = z��
Vector3 OBB::GetDirection(int _element)
{
	return m_normalDirection[_element];
}

Vector3 OBB::GetPositionWorld()
{
	return m_position;
}

// �e���̒������擾 0 = x��, 1 = y��, 2 = z��
float OBB::GetLength(int _element)
{
	return m_length[_element];
}

Matrix4 OBB::GetMatrix()
{
	Matrix4 finalMat, rotmat, scale;

	rotmat.mat[0][0] = m_normalDirection[0].x;
	rotmat.mat[0][1] = m_normalDirection[1].x;
	rotmat.mat[0][2] = m_normalDirection[2].x;
	rotmat.mat[0][3] = 0.0f;
	rotmat.mat[1][0] = m_normalDirection[0].y;
	rotmat.mat[1][1] = m_normalDirection[1].y;
	rotmat.mat[1][2] = m_normalDirection[2].y;
	rotmat.mat[1][3] = 0.0f;
	rotmat.mat[2][0] = m_normalDirection[0].z;
	rotmat.mat[2][1] = m_normalDirection[1].z;
	rotmat.mat[2][2] = m_normalDirection[2].z;
	rotmat.mat[2][3] = 0.0f;
	rotmat.mat[3][0] = 0.0f;
	rotmat.mat[3][1] = 0.0f;
	rotmat.mat[3][2] = 0.0f;
	rotmat.mat[3][3] = 1.0f;

	scale = Matrix4::CreateScale(Vector3(m_length[0], m_length[1], m_length[2]));
	finalMat = scale * rotmat;
	finalMat.mat[3][0] = m_position.x;
	finalMat.mat[3][1] = m_position.y;
	finalMat.mat[3][2] = m_position.z;

	return finalMat;

}

void OBB::Transform(Matrix4 & _matrix)
{
	m_position = Vector3::Transform(m_position, _matrix);

	m_normalDirection[0] = _matrix.GetXAxis();
	m_normalDirection[1] = _matrix.GetYAxis();
	m_normalDirection[2] = _matrix.GetZAxis();
}

// OBB�ǂ����̏Փ�
bool ColOBBs(OBB &_obbA, OBB &_obbB)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	Vector3 NAe1 = _obbA.GetDirection(0), Ae1 = NAe1 * _obbA.GetLength(0);
	Vector3 NAe2 = _obbA.GetDirection(1), Ae2 = NAe2 * _obbA.GetLength(1);
	Vector3 NAe3 = _obbA.GetDirection(2), Ae3 = NAe3 * _obbA.GetLength(2);
	Vector3 NBe1 = _obbB.GetDirection(0), Be1 = NBe1 * _obbB.GetLength(0);
	Vector3 NBe2 = _obbB.GetDirection(1), Be2 = NBe2 * _obbB.GetLength(1);
	Vector3 NBe3 = _obbB.GetDirection(2), Be3 = NBe3 * _obbB.GetLength(2);
	Vector3 Interval = _obbA.GetPositionWorld() - _obbB.GetPositionWorld();

	// ������ : Ae1
	float rA = Ae1.Length();
	float rB = LensSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabs(Vector3::Dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	 // ������ : Ae2
	rA = Ae2.Length();
	rB = LensSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = Ae3.Length();
	rB = LensSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LensSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.Length();
	L = fabs(Vector3::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LensSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.Length();
	L = fabs(Vector3::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LensSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.Length();
	L = fabs(Vector3::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	Vector3 Cross;
	Vector3 Zero = Vector3::Zero;
	Cross = Vector3::Cross(NAe1, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;

}



/// <summary>
/// AABB���m�̏Փ˔���
/// </summary>
/// <returns> 2��AABB���Փ˂��Ă��邩 </returns>
bool Intersect(const AABB & _boxA, const AABB & _boxB)
{
	bool no = _boxA.m_max.x < _boxB.m_min.x ||
	          _boxA.m_max.y < _boxB.m_min.y ||
	          _boxA.m_max.z < _boxB.m_min.z ||
	          _boxB.m_max.x < _boxA.m_min.x ||
	          _boxB.m_max.y < _boxA.m_min.y ||
	          _boxB.m_max.z < _boxA.m_min.z;
	
	// ���true���Ȃ���΁A��͏Փ˂��Ă���
	return !no;
}

// ����AABB�̏Փ�
bool Intersect(const Sphere & _sphere, const AABB & _box)
{
	float distSq = _box.MinDistSq(_sphere.m_center);
	return distSq <= (_sphere.m_radius * _sphere.m_radius);
}

/// <summary>
/// AABB�ƕǖʂ̏Փ˔���
/// </summary>
bool Intersect(const AABB& _box, const Wall& _wall, CollisionInfo& _info_)
{
	// �߂荞�ݖ߂�x�N�g��������
	_info_.m_fixVec = Vector3(0, 0, 0);

	// AABB���m�łԂ���Ȃ��̂ł���Α������^�[��
	if (!Intersect(_box, _wall.m_wallAABB))
	{
		return false;
	}

	// �ǖʂ̃G�b�W�ƏՓ˂����Ƃ�
	Vector3 fix(0, 0, 0);										// �X�^�[�g�_���@�@�@�@�X�^�[�g�_��
	if (IntersectWallEdgeFix(_box, _wall.m_wallVertices[0], _wall.m_wallVertices[1], fix))
	{
		_info_.m_fixVec = fix;
		_info_.m_collisionPoint = _wall.m_wallVertices[0] + fix;
		return true;
	}
	// �I���_���@�@�@�@�@�@�@�I���_��
	if (IntersectWallEdgeFix(_box, _wall.m_wallVertices[3], _wall.m_wallVertices[2], fix))
	{
		_info_.m_fixVec = fix;
		_info_.m_collisionPoint = _wall.m_wallVertices[3] + fix;
		return true;
	}

	// AABB�̒��S�_�͕ǂ̊O���ɂ��邩�H
	Vector3 boxCenter;
	boxCenter = (_box.m_min + _box.m_max) * 0.5f;

	float planeLength;
	planeLength = Vector3::Dot(_wall.m_normal, boxCenter) + _wall.m_planeD;

	// �ǂ̊O���ɂ���Ƃ�
	if (planeLength > 0.0f)
	{
		///////////////////////////////////////////////////////////
		// �ǖʂɑ΂��čł��߂��_�����߂�
		///////////////////////////////////////////////////////////
		Vector3 nearestPoint;

		// ���ʂ���̕����t������8�_���ׂċ��߂�
		std::vector<float> lengths;
		for (int i = 0; i < 8; i++)
		{
			lengths.push_back(Vector3::Dot(_wall.m_normal, _box.m_vertices[i]));
		}
		// �ǖʂɑ΂���AABB�̍ŋߖT�_��������
		auto minitr = std::min_element(lengths.begin(), lengths.end());

		// �{�b�N�X���S�_��ʂ�A�ǖʂƓ����@���̕��ʂ̕������𗧂Ă�
		size_t nearestindex = std::distance(lengths.begin(), minitr);
		nearestPoint = _box.m_vertices[nearestindex];

		//// �ŋߖT�_��Wall��AABB���Ɋ܂܂�邩�H
		//if (!Intersect(wall.mWallAABB, nearestPoint, info))
		//{
		//	return true;
		//}

		// �ŋߖT�_�Ǝ��ۂ̕ǂ̕��ʕ������ŏՓ˔�����s��
		planeLength = Vector3::Dot(_wall.m_normal, nearestPoint) + _wall.m_planeD;

		if (planeLength < 0.0f)
		{
			//�Ԃ������̂ŏՓ˓_�E�����߂��ʂ�Ԃ�
			_info_.m_fixVec = -1.0f * planeLength * _wall.m_normal;
			_info_.m_collisionPoint = nearestPoint + _info_.m_fixVec;

			return true;
		}
	}
	return false;
}

/// <summary>
/// AABB�Ɠ_�̏Փ˔���
/// </summary>
bool Intersect(const AABB& _box, const Vector3& _point, CollisionInfo& _info_)
{
	_info_.m_collisionPoint = Vector3(0, 0, 0);
	_info_.m_fixVec = Vector3(0, 0, 0);

	if (!_box.Contains(_point))
	{
		return false;
	}
	float dx1 = _point.x - _box.m_min.z;
	float dx2 = _point.x - _box.m_min.x;
	float dy1 = _point.y - _box.m_max.x;
	float dy2 = _point.y - _box.m_min.y;
	float dz1 = _point.z - _box.m_max.y;
	float dz2 = _point.z - _box.m_max.z;

	// dx, dy, dz�ɂ��ꂼ���Βl���������ق����Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x,y,z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		_info_.m_fixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		_info_.m_fixVec.y = dy;
	}
	else
	{
		_info_.m_fixVec.z = dz;
	}

	_info_.m_collisionPoint = _point + _info_.m_fixVec;

	return true;
}

/// <summary>
/// ������AABB�̏Փ˔���
/// </summary>
bool Intersect(const Line& _line, const AABB& _box, CollisionInfo& _info_)
{
	// ������AABB�̃x�N�g����z�� x,y,z -> [0],[1],[2]
	float p[3], d[3], min[3], max[3];
	p[0] = _line.m_startPoint.x;
	p[1] = _line.m_startPoint.y;
	p[2] = _line.m_startPoint.z;

	d[0] = _line.m_normalizedDir.x;
	d[1] = _line.m_normalizedDir.y;
	d[2] = _line.m_normalizedDir.z;

	min[0] = _box.m_min.x;
	min[1] = _box.m_min.y;
	min[2] = _box.m_min.z;

	max[0] = _box.m_max.x;
	max[1] = _box.m_max.y;
	max[2] = _box.m_max.z;

	// �����̕������p�����[�^t��float�ŏ��l�A�ő�l�ŏ�����
	float t = -FLT_MAX;
	float t_max = FLT_MAX;

	for (int i = 0; i < 3; ++i)
	{
		if (fabs(d[i]) < FLT_EPSILON)
		{
			if (p[i] < min[i] || p[i] > max[i])
			{
				return false;
			}
		}
		else
		{
			// �X���u�Ƃ̋������Z�o
			// t1���߂��X���u�At2�������X���u�Ƃ̋���
			float odd = 1.0f / d[i];
			float t1 = (min[i] - p[i]) * odd;
			float t2 = (max[i] - p[i]) * odd;
			if (t1 > t2)
			{
				float tmp = t1;
				t1 = t2;
				t2 = tmp;
			}

			if (t1 > t)
			{
				t = t1;
			}
			if (t2 < t_max)
			{
				t_max = t2;
			}
			// �X���u�����`�F�b�N
			if (t >= t_max)
			{
				return false;
			}
		}
	}
	Vector3 lineLength = _line.m_endPoint - _line.m_startPoint;
	if (t < 0.0f || t > lineLength.Length())
	{
		return false;
	}

	//�Փˏ��
	_info_.m_collisionPoint = _line.m_startPoint + t * (_line.m_normalizedDir);

	// �Փ˖@���͒��S�_����Փ˓_�̈�ԑ傫���������画�f����
	Vector3 centerPos, centerToColPos;
	centerPos = 0.5f * (_box.m_min + _box.m_max);
	centerToColPos = _info_.m_collisionPoint - centerPos;

	float elem[3], normal[3];
	elem[0] = fabs(centerToColPos.x);
	elem[1] = fabs(centerToColPos.y);
	elem[2] = fabs(centerToColPos.z);

	//x,y,z�ő�̃C���f�b�N�X�ԍ���Ԃ�
	int index = (elem[0] > elem[1]) ? (elem[0] > elem[2] ? 0 : 2) : (elem[1] > elem[2] ? 1 : 2);

	// �ő�v�f�݂̂̐����c�����0�ɂ��A���̌㐳�K�����邱�ƂŁAx,y,z�̂����ő�v�f�̂�+1/-1��AABB�̖@���ƂȂ�
	for (int i = 0; i < 3; i++)
	{
		normal[i] = (index == i ? elem[i] : 0);
	}
	_info_.m_hitNormal.x = normal[0];
	_info_.m_hitNormal.y = normal[1];
	_info_.m_hitNormal.z = normal[2];

	_info_.m_hitNormal.Normalize();

	return true;
}


/// <summary>
/// �ǖʂƐ����̏Փ˔���
/// </summary>
bool Intersect(const Wall& _wall, const Line& _line, CollisionInfo& _info_)
{
	// ������Wall����镽�ʂ��������邩�H
	Vector3 N, v1, v2;
	N = _wall.m_normal;
	v1 = _line.m_startPoint - _wall.m_wallVertices[0];
	v2 = _line.m_endPoint - _wall.m_wallVertices[0];

	float dotA = Vector3::Dot(v1, N);
	float dotB = Vector3::Dot(v2, N);

	const float epsiron = 0.000001f;
	dotA = fabs(dotA) > epsiron ? dotA : 0.0f;
	dotB = fabs(dotB) > epsiron ? dotB : 0.0f;


	// ���ʂƐ����͏Փ˂��Ȃ�
	if (dotA * dotB > 0.0f)
	{
		return false;
	}

	// �����ƕ��ʂ͌�_������
	_info_.m_hitNormal = _wall.m_normal;

	if (dotA == 0.0f && dotB == 0.0f)
	{
		//���������ʂɊ��S�Ɋ܂܂��ꍇ
		_info_.m_collisionPoint = _line.m_startPoint;
		_info_.m_fixVec = _line.m_startPoint - _line.m_endPoint;

		return true;
	}

	// ���ʂƂ̌�_�����߂�
	Vector3 vec, planeHitPos;
	vec = _line.m_endPoint - _line.m_startPoint;

	//�Փ˂�������̊��������߁A��������Փ˓_�����߂�
	float ratio = fabs(dotA) / (fabs(dotA) + fabs(dotB));
	planeHitPos = vec * ratio + _line.m_startPoint;

	//�Փ˓_��AABB���Ɋ܂܂�Ă�����Փ˂��Ă��� 
	if (_wall.m_wallAABB.Contains(planeHitPos))
	{
		_info_.m_fixVec = _info_.m_collisionPoint - _line.m_endPoint;
		_info_.m_collisionPoint = planeHitPos;
		return true;
	}

	return false;
}

/// <summary>
/// �ǖʂ�AABB�̏Փ˔���(���W�������s��)
/// </summary>
bool IntersectWallEdgeFix(const AABB& _box, const Vector3& _edgeLow, const Vector3& _edgeHi, Vector3& _fixVec_)
{
	_fixVec_ = Vector3(0, 0, 0);

	bool no = _box.m_max.x < _edgeLow.x   ||
	          _box.m_max.y < _edgeLow.y   ||
	          _box.m_max.z < _edgeLow.z   ||
	          _edgeLow.x   < _box.m_min.x ||
	          _edgeLow.y   < _box.m_min.y ||
	          _edgeHi.z    < _box.m_min.z;

	// ���������������Ă��Ȃ��ꍇ�͏I��
	if (no)
	{
		return false;
	}

	float dx1 = _edgeLow.x - _box.m_min.x;
	float dx2 = _edgeLow.x - _box.m_max.x;
	float dy1 = _edgeLow.y - _box.m_max.y;
	float dy2 = _edgeLow.y - _box.m_min.y;

	// dx, dy, dz�ɂ��ꂼ���Βl���������ق����Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

	// x,y,z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy))
	{
		if (Math::Abs(dx) > 0.0001f)
		{
			_fixVec_.x = dx;
		}
	}
	else
	{
		if (Math::Abs(dy) > 0.0001f)
		{
			_fixVec_.y = dy;
		}
	}
	// �C�������܂�ɏ������Ƃ��̓q�b�g���Ă��Ȃ��Ƃ݂Ȃ�
	if (_fixVec_.LengthSq() < 0.0001f)
	{
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------+
// �Փˎ��A�߂荞�񂾕�����߂�����
// in     movableBox �ړ����� (ex �v���C���[)
// in     fixedBox   �ړ����Ȃ����� (ex �u���b�N)
// inout  calcFixVec �ړ����̂̕␳�����x�N�g��
//----------------------------------------------------------------------------------------------------+
void CalcCollisionFixVec(const AABB & _movableBox, const AABB & _fixedBox, Vector3 & _calcFixVec_)
{
	_calcFixVec_ = Vector3(0, 0, 0);
	float dx1 = _fixedBox.m_min.x - _movableBox.m_max.x;
	float dx2 = _fixedBox.m_max.x - _movableBox.m_min.x;
	float dy1 = _fixedBox.m_min.y - _movableBox.m_max.y;
	float dy2 = _fixedBox.m_max.y - _movableBox.m_min.y;
	float dz1 = _fixedBox.m_min.z - _movableBox.m_max.z;
	float dz2 = _fixedBox.m_max.z - _movableBox.m_min.z;

	// dx, dy, dz �ɂ� ���ꂼ��1,2�̂�����Βl�������������Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		_calcFixVec_.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		_calcFixVec_.y = dy;
	}
	else
	{
		_calcFixVec_.z = dz;
	}
}

float PlaneToPointDist(const Vector3 & _position, const Plane _plane)
{
	Vector3 planePos = _plane.m_position;
	Vector3 v;

	v = _position - planePos;
	
	float dist = Vector3::Dot(v, _plane.m_normal);

	return dist;
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o

float LensSegOnSeparateAxis(Vector3 & _separate, Vector3 & _e1, Vector3 & _e2)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
// ������Seq�͕W��������Ă��邱��
	float r1 = fabs(Vector3::Dot(_separate, _e1));
	float r2 = fabs(Vector3::Dot(_separate, _e2));

	return r1 + r2;

}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float LensSegOnSeparateAxis(Vector3 & _separate, Vector3 & _e1, Vector3 & _e2, Vector3 & _e3)
{
	float r1r2 = LensSegOnSeparateAxis(_separate, _e1, _e2);
	float r3 = fabs(Vector3::Dot(_separate, _e3));

	return r1r2 + r3;

}

Line::Line()
	:m_startPoint(0.0f, 0.0f, 0.0f)
	,m_endPoint(0.0f, 0.0f, 0.0f)
	,m_normalizedDir(0.0f, 0.0f, 0.0f)
	,m_lineLength(0.0f)
{
}

Line::Line(const Vector3& _startPoint, const Vector3& _endPoint)
	:m_startPoint(_startPoint)
	,m_endPoint(_endPoint)
	,m_normalizedDir(0.0f, 0.0f, 0.0f)
	,m_lineLength(0.0f)
	,m_rotatable(false)
{
	Vector3 diff;
	diff = m_endPoint - m_startPoint;
	m_lineLength = diff.Length();

	m_normalizedDir = (1.0f / m_lineLength) * diff;
}


Wall::Wall()
	:m_normal(0.0f, 0.0f, 0.0f)
	,m_startPoint(0.0f, 0.0f, 0.0f)
	,m_endPoint(0.0f, 0.0f, 0.0f)
	,m_planeD(0.0f)
	,m_scale(0.0f, 0.0f, 0.0f)
	,m_zRotate(0.0f)
	,m_slopeAngle(0.0f)
	,m_isSlope(false)
{
	for (int i = 0; i < 4; i++)
	{
		m_wallVertices[i] = Vector3(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// ���W���ǂƓ������Ă��邩
/// </summary>
/// <param name="_pos"> ���ׂ������W�|�C���g </param>
bool Wall::Contains(const Vector3& _pos)
{
	// AABB�ɂ��܂܂�Ȃ��ꍇ�͏I��
	if (!m_wallAABB.Contains(_pos))
	{
		return false;
	}

	// ���ʂ̕����� P�EN + D = 0 ���
	if (Vector3::Dot(m_normal, _pos) + m_planeD > 0.0f)
	{
		return false;
	}

	return true;
}

/// <summary>
/// �ǖʓ����蔻��̍쐬
/// </summary>
/// <param name="_start"> �ǂ̊J�n�n�_ </param>
/// <param name="_end"> �ǂ̏I���n�_ </param>
/// <param name="_height"> �ǂ̍��� </param>
/// <returns> �ǖʂƂ��Ē�`���������蔻��\���� </returns>
Wall Wall::CreateWall(const Vector3& _start, const Vector3& _end, float _height)
{
	Wall temp;

	temp.m_startPoint = _start;
	temp.m_endPoint = _end;
	temp.m_isSlope = false;

	// �ǖʂ̕\�ʃx�N�g�������߂�
	Vector3 wallLine = _end - _start;
	Vector3 nomalizedWallLine = wallLine;
	nomalizedWallLine.Normalize();

	// �ǖʂ̕��ʕ�����
	temp.m_normal = Vector3::Cross(Vector3::UnitZ, nomalizedWallLine);
	temp.m_planeD = -1.0f * Vector3::Dot(temp.m_normal, _start);

	// �ǖʂ�4�_�����߂�(start�_�̉����とend�_�̏と���̏�)
	Vector3 zMax(0.0f, 0.0f, _start.z + _height);

	temp.m_wallVertices[0] = _start;
	temp.m_wallVertices[1] = _start + zMax;
	temp.m_wallVertices[2] = _end + zMax;
	temp.m_wallVertices[3] = _end;

	temp.m_scale.x = 100.0f;
	temp.m_scale.y = (_end - _start).Length();
	temp.m_scale.z = _height;

	// �\����]�p�Ɖ�]����
	Vector3 sgnVec = Vector3::Cross(Vector3::UnitX, nomalizedWallLine);
	float zAngle = acosf(Vector3::Dot(Vector3::UnitX, nomalizedWallLine)) - Math::PiOver2;

	temp.m_zRotate = sgnVec.z > 0 ? zAngle : -zAngle + Math::Pi;
	temp.CalcAABB();

	return temp;
}

/// <summary>
/// �X���[�v(�X��)�̍쐬
/// </summary>
/// <param name="_pos"> �X�΂�z�u������W </param>
/// <param name="_unitZDegree"> Z���p�x </param>
/// <param name="_horizonLength"> ���������̒��� </param>
/// <param name="_height"> �X�΍��� </param>
/// <param name="_width"> �X�΂̕� </param>
/// <returns> �X�΂Ƃ��Ē�`�����ǖʓ����蔻�� </returns>
Wall Wall::CreateSlope(const Vector3& _pos, Wall::WALL_ANGLE _unitZDegree, float _horizonLength, float _height, float _width)
{
	Wall temp;
	Vector3 slopeLine, normalizedSlopeLine;

	// �X���[�v�ʂ��ォ�猩���������A�E���ɂȂ�x�N�g��
	Vector3 slopeRightVec(0.0f, 1.0f, 0.0f);

	temp.m_startPoint = Vector3(0, 0, _height);
	temp.m_endPoint = Vector3(_horizonLength, 0, 0);
	temp.m_isSlope = true;

	// �X���[�v�̕\�ʃx�N�g�������߂�
	slopeLine = temp.m_endPoint - temp.m_startPoint;
	normalizedSlopeLine = slopeLine;
	normalizedSlopeLine.Normalize();

	// �ǖʖ@���x�N�g�����߂�
	temp.m_normal = Vector3::Cross(normalizedSlopeLine, slopeRightVec);
	temp.m_normal.Normalize();

	//�X�Ίp�����߂�
	temp.m_slopeAngle = acosf(Vector3::Dot(Vector3::UnitX, normalizedSlopeLine)) + Math::PiOver2;

	// �X���[�v�ʂ�4���_�����߂�
	float halfWidth = _width * 0.5f;
	temp.m_wallVertices[0] = temp.m_startPoint - slopeRightVec * halfWidth;
	temp.m_wallVertices[1] = temp.m_startPoint + slopeRightVec * halfWidth;
	temp.m_wallVertices[2] = temp.m_endPoint + slopeRightVec * halfWidth;
	temp.m_wallVertices[3] = temp.m_endPoint - slopeRightVec * halfWidth;

	temp.m_scale.x = 100.0f;
	temp.m_scale.y = _width;
	temp.m_scale.z = (temp.m_endPoint - temp.m_startPoint).Length();

	temp.m_zRotate = Math::ToRadians(static_cast<float>(_unitZDegree));

	// �X���[�v�ʂ�4���_�̃��[�J��Z��] + ���s�ړ�
	Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(static_cast<float>(_unitZDegree)));
	Matrix4 trans = Matrix4::CreateTranslation(_pos);
	Matrix4 mat = rot * trans;
	for (int i = 0; i < 4; i++)
	{
		temp.m_wallVertices[i] = Vector3::Transform(temp.m_wallVertices[i], mat);
	}
	// �@������]
	temp.m_normal = Vector3::Transform(temp.m_normal, rot);
	temp.m_planeD = -1.0f * Vector3::Dot(temp.m_normal, temp.m_wallVertices[0]);

	temp.CalcAABB();

	return temp;
}

/// <summary>
/// �ǖʂ�AABB���`����
/// </summary>
void Wall::CalcAABB()
{
	// x,y,z�̍ő�l�@�ŏ��l�𒲂ׂ�
	m_wallAABB.m_min = m_wallAABB.m_max = m_wallVertices[0];
	for (int i = 1; i < 4; i++)
	{
		m_wallAABB.m_min.x = Math::Min(m_wallAABB.m_min.x, m_wallVertices[i].x);
		m_wallAABB.m_min.y = Math::Min(m_wallAABB.m_min.y, m_wallVertices[i].y);
		m_wallAABB.m_min.z = Math::Min(m_wallAABB.m_min.z, m_wallVertices[i].z);

		m_wallAABB.m_max.x = Math::Max(m_wallAABB.m_max.x, m_wallVertices[i].x);
		m_wallAABB.m_max.y = Math::Max(m_wallAABB.m_max.y, m_wallVertices[i].y);
		m_wallAABB.m_max.z = Math::Max(m_wallAABB.m_max.z, m_wallVertices[i].z);
	}
}
