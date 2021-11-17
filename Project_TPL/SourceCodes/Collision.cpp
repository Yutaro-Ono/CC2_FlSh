//-----------------------------------------------------------------------+
// 衝突検知(当たり判定)クラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Collision.h"
#include <algorithm>
#include <array>

float LensSegOnSeparateAxis(Vector3& _separate, Vector3& _e1, Vector3& _e2, Vector3& _e3);     // 分離軸に投影された軸成分から投影線分長を算出
float LensSegOnSeparateAxis(Vector3& _separate, Vector3& _e1, Vector3& _e2);                     // 分離軸に投影された軸成分から投影線分長を算出

//-------------------------------------------------------------+
// 球体
//-------------------------------------------------------------+
// コンストラクタ
// (mCenter:初期化, mRdius:初期化)
Sphere::Sphere(const Vector3 & _center, float _radius)
	:m_center(_center)
	,m_radius(_radius)
{
	// 処理なし
}

// 球体当たり判定処理(Sphereの中にpointが含まれているか)
bool Sphere::Contains(const Vector3 & _point) const
{
	// 球の中心と点との距離の二乗
	float distSq = (m_center - _point).LengthSq();
	// ↑と半径の2乗を比較し、返す
	return distSq <= m_radius * m_radius;
}

// 球体同士の当たり判定
bool Sphere::ContainsSphere(const Sphere _otherSphere) const
{
	// 球体と球体の中心点の距離の二乗を取得
	float distSq = (m_center - _otherSphere.m_center).LengthSq();
	// 球体同士の半径の二乗から比較し、円と円が触れているかを返す
	return distSq <= (m_radius + _otherSphere.m_radius) * (m_radius + _otherSphere.m_radius);
}


//-------------------------------------------------------------+
// 平面
//-------------------------------------------------------------+
// コンストラクタ
// (mNormal:初期化, mPos:初期化
Plane::Plane(const Vector3 & _normal, const Vector3 _position)
	:m_normal(_normal)
	,m_position(_position)
{
	// 処理なし
}


//-------------------------------------------------------------+
// 軸並行ボックス (AABB:Axis-Aligned Bounding Box)
//-------------------------------------------------------------+
// コンストラクタ
AABB::AABB()
{
	m_min = Vector3(0.0f, 0.0f, 0.0f);
	m_max = Vector3(0.0f, 0.0f, 0.0f);
}

// 初期化用コンストラクタ
AABB::AABB(const Vector3 & _min, const Vector3 & _max)
	:m_min(_min)
	,m_max(_max)
{
	// 頂点のセット
	SetBoxVertex();
}

/// <summary>
/// Boxを構成する8頂点のセット
/// </summary>
void AABB::SetBoxVertex()
{
	// 箱の上側4頂点
	m_vertices[0] = Vector3(m_min.x, m_min.y, m_max.z);
	m_vertices[1] = Vector3(m_max.x, m_min.y, m_max.z);
	m_vertices[2] = Vector3(m_min.x, m_max.y, m_max.z);
	m_vertices[3] = Vector3(m_max.x, m_max.y, m_max.z);

	// 箱の下側4頂点
	m_vertices[4] = Vector3(m_min.x, m_min.y, m_min.z);
	m_vertices[5] = Vector3(m_max.x, m_min.y, m_min.z);
	m_vertices[6] = Vector3(m_min.x, m_max.y, m_min.z);
	m_vertices[7] = Vector3(m_max.x, m_max.y, m_min.z);
}

// 最小値と最大値を更新
void AABB::UpdateMinMax(const Vector3 & _point)
{

	// x,y,zそれぞれの最小値を求める
	m_min.x = Math::Min(m_min.x, _point.x);
	m_min.y = Math::Min(m_min.y, _point.y);
	m_min.z = Math::Min(m_min.z, _point.z);

	// x,y,zそれぞれの最大値を求める
	m_max.x = Math::Max(m_max.x, _point.x);
	m_max.y = Math::Max(m_max.y, _point.y);
	m_max.z = Math::Max(m_max.z, _point.z);

}

/// <summary>
/// Box内に指定した点が入っているか
/// </summary>
/// <param name="_point"> 調べる座標ポイント </param>
/// <returns> 内側に点が存在するか </returns>
bool AABB::Contains(const Vector3 & _point) const
{
	
	bool outside = _point.x < m_min.x ||
		_point.y < m_min.y ||
		_point.z < m_min.z ||
		_point.x > m_max.x ||
		_point.y > m_max.y ||
		_point.z > m_max.z;

	// もし一つもtrueがなければ、boxの内側に点が存在する
	return !outside;
}

/// <summary>
/// 点とAABBの距離(8頂点から見た最小距離)
/// </summary>
/// <param name="_point"></param>
/// <returns></returns>
float AABB::MinDistSq(const Vector3 & _point) const
{

	// それぞれの軸での差をとる
	float dx = Math::Max(m_min.x - _point.x, 0.0f);
	dx = Math::Max(dx, _point.x - m_max.x);
	float dy = Math::Max(m_min.y - _point.y, 0.0f);
	dy = Math::Max(dy, _point.y - m_max.y);
	float dz = Math::Max(m_min.z - _point.z, 0.0f);
	dz = Math::Max(dy, _point.z - m_max.z);

	// 3次元空間での距離の二乗の公式より
	return dx * dx + dy * dy + dz * dz;
}

/// <summary>
/// AABBの回転処理
/// </summary>
/// <param name="_quaternion"> 回転行列 </param>
void AABB::Rotate(const Quaternion & _quaternion)
{
	// ボックスの8つの頂点の配列作る
	std::array<Vector3, 8> points;
	// 最小値は常にコーナーである
	points[0] = m_min;
	// 2つの最小値と1個の最大値の並べ替え
	points[1] = Vector3(m_max.x, m_min.y, m_min.z);
	points[2] = Vector3(m_min.x, m_max.y, m_min.z);
	points[3] = Vector3(m_min.x, m_min.y, m_max.z);
	// 2つの最大値と1個の最小値の並べ替え
	points[4] = Vector3(m_min.x, m_max.y, m_max.z);
	points[5] = Vector3(m_max.x, m_min.y, m_max.z);
	points[6] = Vector3(m_max.x, m_max.y, m_min.z);
	// 最大値は常にコーナーである
	points[7] = Vector3(m_max);

	// 最初の点を回転
	Vector3 p = Vector3::Transform(points[0], _quaternion);
	// 最小値と最大値を求めるために最初の点を入れてリセット
	m_min = p;
	m_max = p;
	// 回転によって最大値と最小値を求めなおす。
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], _quaternion);
		UpdateMinMax(p);
	}
}

/// <summary>
/// AABBのスケーリング
/// </summary>
/// <param name="_scaleX"> X拡大率 </param>
/// <param name="_scaleY"> Y拡大率 </param>
/// <param name="_scaleZ"> Z拡大率 </param>
void AABB::SetScale(float _scaleX, float _scaleY, float _scaleZ)
{
	m_max = m_max * Vector3(_scaleX, _scaleY, _scaleZ);
	m_min = m_min * Vector3(_scaleX, _scaleY, _scaleZ);
}


//----------------------------------------------------+
// 有向ボックス (OBB:Oriented Bounding Box)
//----------------------------------------------------+
// コンストラクタ
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

	// 回転角(度数)よりラジアン角に変換
	theta[0] = Math::ToRadians(_degree.x);
	theta[1] = Math::ToRadians(_degree.y);
	theta[2] = Math::ToRadians(_degree.z);

	// 回転角より回転行列作成
	matrix[0] = Matrix4::CreateRotationX(theta[0]);
	matrix[1] = Matrix4::CreateRotationY(theta[1]);
	matrix[2] = Matrix4::CreateRotationZ(theta[2]);

	// 回転行列の合成より基底ベクトルを取得
	finalMat = matrix[0] * matrix[1] * matrix[2];

	m_normalDirection[0] = finalMat.GetXAxis();
	m_normalDirection[1] = finalMat.GetYAxis();
	m_normalDirection[2] = finalMat.GetZAxis();

	// スケール(ボックス長さ)の半分を保存
	m_length[0] = _scale.x * 0.5f;
	m_length[1] = _scale.y * 0.5f;
	m_length[2] = _scale.z * 0.5f;

}

// AABBを初期値にしてOBBを作る
OBB::OBB(AABB _aabb)
{

	// 回転なし
	m_normalDirection[0] = Vector3::UnitX;
	m_normalDirection[1] = Vector3::UnitY;
	m_normalDirection[2] = Vector3::UnitZ;

	// 各軸の長さ
	m_length[0] = (_aabb.m_max.x - _aabb.m_min.x) * 0.5f;
	m_length[1] = (_aabb.m_max.y - _aabb.m_min.y) * 0.5f;
	m_length[2] = (_aabb.m_max.z - _aabb.m_min.z) * 0.5f;

	// ボックス中心のワールド座標
	m_position.x = _aabb.m_min.x + m_length[0];
	m_position.y = _aabb.m_min.y + m_length[1];
	m_position.z = _aabb.m_min.z + m_length[2];

}

// 各軸の向きを取得 0 = x軸, 1 = y軸, 2 = z軸
Vector3 OBB::GetDirection(int _element)
{
	return m_normalDirection[_element];
}

Vector3 OBB::GetPositionWorld()
{
	return m_position;
}

// 各軸の長さを取得 0 = x軸, 1 = y軸, 2 = z軸
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

// OBBどうしの衝突
bool ColOBBs(OBB &_obbA, OBB &_obbB)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	Vector3 NAe1 = _obbA.GetDirection(0), Ae1 = NAe1 * _obbA.GetLength(0);
	Vector3 NAe2 = _obbA.GetDirection(1), Ae2 = NAe2 * _obbA.GetLength(1);
	Vector3 NAe3 = _obbA.GetDirection(2), Ae3 = NAe3 * _obbA.GetLength(2);
	Vector3 NBe1 = _obbB.GetDirection(0), Be1 = NBe1 * _obbB.GetLength(0);
	Vector3 NBe2 = _obbB.GetDirection(1), Be2 = NBe2 * _obbB.GetLength(1);
	Vector3 NBe3 = _obbB.GetDirection(2), Be3 = NBe3 * _obbB.GetLength(2);
	Vector3 Interval = _obbA.GetPositionWorld() - _obbB.GetPositionWorld();

	// 分離軸 : Ae1
	float rA = Ae1.Length();
	float rB = LensSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabs(Vector3::Dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : Ae2
	rA = Ae2.Length();
	rB = LensSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = Ae3.Length();
	rB = LensSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LensSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.Length();
	L = fabs(Vector3::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LensSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.Length();
	L = fabs(Vector3::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LensSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.Length();
	L = fabs(Vector3::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	Vector3 Cross;
	Vector3 Zero = Vector3::Zero;
	Cross = Vector3::Cross(NAe1, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;

}



/// <summary>
/// AABB同士の衝突判定
/// </summary>
/// <returns> 2つのAABBが衝突しているか </returns>
bool Intersect(const AABB & _boxA, const AABB & _boxB)
{
	bool no = _boxA.m_max.x < _boxB.m_min.x ||
	          _boxA.m_max.y < _boxB.m_min.y ||
	          _boxA.m_max.z < _boxB.m_min.z ||
	          _boxB.m_max.x < _boxA.m_min.x ||
	          _boxB.m_max.y < _boxA.m_min.y ||
	          _boxB.m_max.z < _boxA.m_min.z;
	
	// 一つもtrueがなければ、二つは衝突している
	return !no;
}

// 球とAABBの衝突
bool Intersect(const Sphere & _sphere, const AABB & _box)
{
	float distSq = _box.MinDistSq(_sphere.m_center);
	return distSq <= (_sphere.m_radius * _sphere.m_radius);
}

/// <summary>
/// AABBと壁面の衝突判定
/// </summary>
bool Intersect(const AABB& _box, const Wall& _wall, CollisionInfo& _info_)
{
	// めり込み戻りベクトル初期化
	_info_.m_fixVec = Vector3(0, 0, 0);

	// AABB同士でぶつからないのであれば早期リターン
	if (!Intersect(_box, _wall.m_wallAABB))
	{
		return false;
	}

	// 壁面のエッジと衝突したとき
	Vector3 fix(0, 0, 0);										// スタート点下　　　　スタート点上
	if (IntersectWallEdgeFix(_box, _wall.m_wallVertices[0], _wall.m_wallVertices[1], fix))
	{
		_info_.m_fixVec = fix;
		_info_.m_collisionPoint = _wall.m_wallVertices[0] + fix;
		return true;
	}
	// 終了点下　　　　　　　終了点上
	if (IntersectWallEdgeFix(_box, _wall.m_wallVertices[3], _wall.m_wallVertices[2], fix))
	{
		_info_.m_fixVec = fix;
		_info_.m_collisionPoint = _wall.m_wallVertices[3] + fix;
		return true;
	}

	// AABBの中心点は壁の外側にいるか？
	Vector3 boxCenter;
	boxCenter = (_box.m_min + _box.m_max) * 0.5f;

	float planeLength;
	planeLength = Vector3::Dot(_wall.m_normal, boxCenter) + _wall.m_planeD;

	// 壁の外側にいるとき
	if (planeLength > 0.0f)
	{
		///////////////////////////////////////////////////////////
		// 壁面に対して最も近い点を求める
		///////////////////////////////////////////////////////////
		Vector3 nearestPoint;

		// 平面からの符号付距離を8点すべて求める
		std::vector<float> lengths;
		for (int i = 0; i < 8; i++)
		{
			lengths.push_back(Vector3::Dot(_wall.m_normal, _box.m_vertices[i]));
		}
		// 壁面に対してAABBの最近傍点を見つける
		auto minitr = std::min_element(lengths.begin(), lengths.end());

		// ボックス中心点を通る、壁面と同じ法線の平面の方程式を立てる
		size_t nearestindex = std::distance(lengths.begin(), minitr);
		nearestPoint = _box.m_vertices[nearestindex];

		//// 最近傍点はWallのAABB内に含まれるか？
		//if (!Intersect(wall.mWallAABB, nearestPoint, info))
		//{
		//	return true;
		//}

		// 最近傍点と実際の壁の平面方程式で衝突判定を行う
		planeLength = Vector3::Dot(_wall.m_normal, nearestPoint) + _wall.m_planeD;

		if (planeLength < 0.0f)
		{
			//ぶつかったので衝突点・押し戻し量を返す
			_info_.m_fixVec = -1.0f * planeLength * _wall.m_normal;
			_info_.m_collisionPoint = nearestPoint + _info_.m_fixVec;

			return true;
		}
	}
	return false;
}

/// <summary>
/// AABBと点の衝突判定
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

	// dx, dy, dzにそれぞれ絶対値が小さいほうをセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x,y,zのうち最も差が小さい軸で位置を調整
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
/// 線分とAABBの衝突判定
/// </summary>
bool Intersect(const Line& _line, const AABB& _box, CollisionInfo& _info_)
{
	// 線分とAABBのベクトルを配列化 x,y,z -> [0],[1],[2]
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

	// 線分の方程式パラメータtをfloat最小値、最大値で初期化
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
			// スラブとの距離を算出
			// t1が近いスラブ、t2が遠いスラブとの距離
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
			// スラブ交差チェック
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

	//衝突情報
	_info_.m_collisionPoint = _line.m_startPoint + t * (_line.m_normalizedDir);

	// 衝突法線は中心点から衝突点の一番大きい成分から判断する
	Vector3 centerPos, centerToColPos;
	centerPos = 0.5f * (_box.m_min + _box.m_max);
	centerToColPos = _info_.m_collisionPoint - centerPos;

	float elem[3], normal[3];
	elem[0] = fabs(centerToColPos.x);
	elem[1] = fabs(centerToColPos.y);
	elem[2] = fabs(centerToColPos.z);

	//x,y,z最大のインデックス番号を返す
	int index = (elem[0] > elem[1]) ? (elem[0] > elem[2] ? 0 : 2) : (elem[1] > elem[2] ? 1 : 2);

	// 最大要素のみの成分残し後は0にし、その後正規化することで、x,y,zのうち最大要素のみ+1/-1のAABBの法線となる
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
/// 壁面と線分の衝突判定
/// </summary>
bool Intersect(const Wall& _wall, const Line& _line, CollisionInfo& _info_)
{
	// 線分とWallが作る平面が交差するか？
	Vector3 N, v1, v2;
	N = _wall.m_normal;
	v1 = _line.m_startPoint - _wall.m_wallVertices[0];
	v2 = _line.m_endPoint - _wall.m_wallVertices[0];

	float dotA = Vector3::Dot(v1, N);
	float dotB = Vector3::Dot(v2, N);

	const float epsiron = 0.000001f;
	dotA = fabs(dotA) > epsiron ? dotA : 0.0f;
	dotB = fabs(dotB) > epsiron ? dotB : 0.0f;


	// 平面と線分は衝突しない
	if (dotA * dotB > 0.0f)
	{
		return false;
	}

	// 線分と平面は交点を持つ
	_info_.m_hitNormal = _wall.m_normal;

	if (dotA == 0.0f && dotB == 0.0f)
	{
		//線分が平面に完全に含まれる場合
		_info_.m_collisionPoint = _line.m_startPoint;
		_info_.m_fixVec = _line.m_startPoint - _line.m_endPoint;

		return true;
	}

	// 平面との交点を求める
	Vector3 vec, planeHitPos;
	vec = _line.m_endPoint - _line.m_startPoint;

	//衝突する内分の割合を求め、そこから衝突点を求める
	float ratio = fabs(dotA) / (fabs(dotA) + fabs(dotB));
	planeHitPos = vec * ratio + _line.m_startPoint;

	//衝突点がAABB内に含まれていたら衝突している 
	if (_wall.m_wallAABB.Contains(planeHitPos))
	{
		_info_.m_fixVec = _info_.m_collisionPoint - _line.m_endPoint;
		_info_.m_collisionPoint = planeHitPos;
		return true;
	}

	return false;
}

/// <summary>
/// 壁面とAABBの衝突判定(座標調整も行う)
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

	// そもそも当たっていない場合は終了
	if (no)
	{
		return false;
	}

	float dx1 = _edgeLow.x - _box.m_min.x;
	float dx2 = _edgeLow.x - _box.m_max.x;
	float dy1 = _edgeLow.y - _box.m_max.y;
	float dy2 = _edgeLow.y - _box.m_min.y;

	// dx, dy, dzにそれぞれ絶対値が小さいほうをセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

	// x,y,zのうち最も差が小さい軸で位置を調整
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
	// 修正があまりに小さいときはヒットしていないとみなす
	if (_fixVec_.LengthSq() < 0.0001f)
	{
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------+
// 衝突時、めり込んだ部分を戻す処理
// in     movableBox 移動物体 (ex プレイヤー)
// in     fixedBox   移動しない物体 (ex ブロック)
// inout  calcFixVec 移動物体の補正差分ベクトル
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

	// dx, dy, dz には それぞれ1,2のうち絶対値が小さい方をセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, zのうち最も差が小さい軸で位置を調整
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

// 分離軸に投影された軸成分から投影線分長を算出

float LensSegOnSeparateAxis(Vector3 & _separate, Vector3 & _e1, Vector3 & _e2)
{
	// 3つの内積の絶対値の和で投影線分長を計算
// 分離軸Seqは標準化されていること
	float r1 = fabs(Vector3::Dot(_separate, _e1));
	float r2 = fabs(Vector3::Dot(_separate, _e2));

	return r1 + r2;

}

// 分離軸に投影された軸成分から投影線分長を算出
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
/// 座標が壁と当たっているか
/// </summary>
/// <param name="_pos"> 調べたい座標ポイント </param>
bool Wall::Contains(const Vector3& _pos)
{
	// AABBにも含まれない場合は終了
	if (!m_wallAABB.Contains(_pos))
	{
		return false;
	}

	// 平面の方程式 P・N + D = 0 より
	if (Vector3::Dot(m_normal, _pos) + m_planeD > 0.0f)
	{
		return false;
	}

	return true;
}

/// <summary>
/// 壁面当たり判定の作成
/// </summary>
/// <param name="_start"> 壁の開始地点 </param>
/// <param name="_end"> 壁の終了地点 </param>
/// <param name="_height"> 壁の高さ </param>
/// <returns> 壁面として定義した当たり判定構造体 </returns>
Wall Wall::CreateWall(const Vector3& _start, const Vector3& _end, float _height)
{
	Wall temp;

	temp.m_startPoint = _start;
	temp.m_endPoint = _end;
	temp.m_isSlope = false;

	// 壁面の表面ベクトルを求める
	Vector3 wallLine = _end - _start;
	Vector3 nomalizedWallLine = wallLine;
	nomalizedWallLine.Normalize();

	// 壁面の平面方程式
	temp.m_normal = Vector3::Cross(Vector3::UnitZ, nomalizedWallLine);
	temp.m_planeD = -1.0f * Vector3::Dot(temp.m_normal, _start);

	// 壁面の4点を求める(start点の下→上→end点の上→下の順)
	Vector3 zMax(0.0f, 0.0f, _start.z + _height);

	temp.m_wallVertices[0] = _start;
	temp.m_wallVertices[1] = _start + zMax;
	temp.m_wallVertices[2] = _end + zMax;
	temp.m_wallVertices[3] = _end;

	temp.m_scale.x = 100.0f;
	temp.m_scale.y = (_end - _start).Length();
	temp.m_scale.z = _height;

	// 表示回転角と回転方向
	Vector3 sgnVec = Vector3::Cross(Vector3::UnitX, nomalizedWallLine);
	float zAngle = acosf(Vector3::Dot(Vector3::UnitX, nomalizedWallLine)) - Math::PiOver2;

	temp.m_zRotate = sgnVec.z > 0 ? zAngle : -zAngle + Math::Pi;
	temp.CalcAABB();

	return temp;
}

/// <summary>
/// スロープ(傾斜)の作成
/// </summary>
/// <param name="_pos"> 傾斜を配置する座標 </param>
/// <param name="_unitZDegree"> Z軸角度 </param>
/// <param name="_horizonLength"> 水平方向の長さ </param>
/// <param name="_height"> 傾斜高さ </param>
/// <param name="_width"> 傾斜の幅 </param>
/// <returns> 傾斜として定義した壁面当たり判定 </returns>
Wall Wall::CreateSlope(const Vector3& _pos, Wall::WALL_ANGLE _unitZDegree, float _horizonLength, float _height, float _width)
{
	Wall temp;
	Vector3 slopeLine, normalizedSlopeLine;

	// スロープ面を上から見下げた時、右側になるベクトル
	Vector3 slopeRightVec(0.0f, 1.0f, 0.0f);

	temp.m_startPoint = Vector3(0, 0, _height);
	temp.m_endPoint = Vector3(_horizonLength, 0, 0);
	temp.m_isSlope = true;

	// スロープの表面ベクトルを求める
	slopeLine = temp.m_endPoint - temp.m_startPoint;
	normalizedSlopeLine = slopeLine;
	normalizedSlopeLine.Normalize();

	// 壁面法線ベクトル求める
	temp.m_normal = Vector3::Cross(normalizedSlopeLine, slopeRightVec);
	temp.m_normal.Normalize();

	//傾斜角を求める
	temp.m_slopeAngle = acosf(Vector3::Dot(Vector3::UnitX, normalizedSlopeLine)) + Math::PiOver2;

	// スロープ面の4頂点を求める
	float halfWidth = _width * 0.5f;
	temp.m_wallVertices[0] = temp.m_startPoint - slopeRightVec * halfWidth;
	temp.m_wallVertices[1] = temp.m_startPoint + slopeRightVec * halfWidth;
	temp.m_wallVertices[2] = temp.m_endPoint + slopeRightVec * halfWidth;
	temp.m_wallVertices[3] = temp.m_endPoint - slopeRightVec * halfWidth;

	temp.m_scale.x = 100.0f;
	temp.m_scale.y = _width;
	temp.m_scale.z = (temp.m_endPoint - temp.m_startPoint).Length();

	temp.m_zRotate = Math::ToRadians(static_cast<float>(_unitZDegree));

	// スロープ面の4頂点のローカルZ回転 + 平行移動
	Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(static_cast<float>(_unitZDegree)));
	Matrix4 trans = Matrix4::CreateTranslation(_pos);
	Matrix4 mat = rot * trans;
	for (int i = 0; i < 4; i++)
	{
		temp.m_wallVertices[i] = Vector3::Transform(temp.m_wallVertices[i], mat);
	}
	// 法線を回転
	temp.m_normal = Vector3::Transform(temp.m_normal, rot);
	temp.m_planeD = -1.0f * Vector3::Dot(temp.m_normal, temp.m_wallVertices[0]);

	temp.CalcAABB();

	return temp;
}

/// <summary>
/// 壁面のAABBを定義する
/// </summary>
void Wall::CalcAABB()
{
	// x,y,zの最大値　最小値を調べる
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
