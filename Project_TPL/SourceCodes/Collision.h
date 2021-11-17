//----------------------------------------------------------------------------------+
// @file        Collision.h
// @brief       当たり判定クラス
// @note        各当たり判定に必要な情報と処理を定義
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2019         新規作成
// 2021/11/17   CollisionInfo構造体を定義
//----------------------------------------------------------------------------------+
#pragma once
#include "GameMain.h"


//----------------------------------------------------+
// 衝突情報総合
//----------------------------------------------------+
typedef struct CollisionInfo
{
	Vector3 m_collisionPoint;       // 衝突点
	Vector3 m_fixVec;               // 当たり判定による修正ベクトル(押し戻し)
	Vector3 m_hitNormal;            // 衝突点における法線
};


//----------------------------------------------------+
// 線分の当たり判定
//----------------------------------------------------+
typedef struct Line
{
	Line();
	Line(const Vector3& _startPoint, const Vector3& _endPoint);

	Vector3 m_startPoint;        // 線分の始点
	Vector3 m_endPoint;          // 線分の終点

	Vector3 m_normalizedDir;     // 正規化した方向ベクトル
	float m_lineLength;          // 線分の長さ
	bool m_rotatable;            // 回転の可否
};


//----------------------------------------------------+
// 壁(スロープも)の当たり判定
//----------------------------------------------------+
typedef struct Wall
{
	// 壁の角度定義用列挙
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


	AABB m_wallAABB;                // 壁全体のAABB
	Vector3 m_normal;               // 壁面の法線
	Vector3 m_startPoint;           // 壁の開始点
	Vector3 m_endPoint;             // 壁の終了点
	Vector3 m_wallVertices[4];          // 壁を構成する4頂点の座標
	Vector3 m_scale;                // 描画時使用(x, y, zスケール)

	float m_planeD;                 // 壁面の平面方程式に用いるD値
	float m_zRotate;                // Z軸回転値
	float m_slopeAngle;             // スロープ時の傾斜角

	bool m_isSlope;                 // スロープかどうか
};

//----------------------------------------------------+
// 球体
//----------------------------------------------------+
typedef struct Sphere
{

	Sphere(const Vector3& _center, float _radius);                                      // コンストラクタ
	bool Contains(const Vector3& _point) const;                                           // ポイント(座標)が球体内に含まれているか
	bool ContainsSphere(const Sphere _otherSphere) const;                                 // 相手の球体とこの球体が接触しているか

	Vector3      m_center;                                                                  // 球体の中心座標
	float        m_radius;                                                                  // 球体の半径

}Sphere;


//----------------------------------------------------+
// 平面
//----------------------------------------------------+
typedef struct Plane
{

	Plane(const Vector3& _normal, const Vector3 _position);                             // コンストラクタ

	Vector3      m_normal;                                                                  // 法線
	Vector3      m_position;                                                                // 平面上の点
	
}Plane;


//----------------------------------------------------+
// 軸並行ボックス (AABB:Axis-Aligned Bounding Box)
//----------------------------------------------------+
typedef struct AABB
{

	AABB();                                                                                 // コンストラクタ
	AABB(const Vector3& _min, const Vector3& _max);                                     // 初期化用コンストラクタ

	
	void         InitMinMax(const Vector3& _point) { m_min = m_max = _point; }          // モデル読み込み時の最小最大の点を求める
	void         SetBoxVertex();                                                          // Boxを構成する8頂点のセット
	
	void         UpdateMinMax(const Vector3& _point);                                     // 最小最大の点を更新

	bool         Contains(const Vector3& _point) const;                                   // 点がボックス内に含まれるか
	float        MinDistSq(const Vector3& _point) const;                                  // 点との最小距離を求める

	void         Rotate(const Quaternion& _quaternion);                                   // 回転
	void         SetAllowRotate(bool _value) { m_isRotatable = _value; }                  // 回転を許可するか

	void         SetScale(float _scaleX, float _scaleY, float _scaleZ);                   // AABBのスケーリング

	// 直接頂点座標をセット
	void         SetMinVector(const Vector3& _minVec) { m_min = _minVec; }
	void         SetMaxVector(const Vector3& _maxVec) { m_max = _maxVec; }

	Vector3      m_min;                                                                     // 最小値
	Vector3      m_max;                                                                     // 最大値
	Vector3      m_vertices[8];                                                             // Boxを構成する8頂点

	bool         m_isRotatable  =  false;                                                   // 回転するかどうか

}AABB;


//----------------------------------------------------+
// 有向ボックス (OBB:Oriented Bounding Box)
//----------------------------------------------------+
typedef struct OBB
{

	OBB();                                                                                  // コンストラクタ
	OBB(const Vector3& _position, const Vector3& _degree, const Vector3& _scale);     // 初期化用コンストラクタ
	OBB(AABB _aabb);                                                                      // 

	Vector3     GetDirection(int _element);                                               // 
	Vector3     GetPositionWorld();                                                         // 

	float       GetLength(int _element);                                                  // 
	Matrix4     GetMatrix();                                                                // 
	void        Transform(Matrix4& _matrix);                                              // 

	Vector3     m_position;                                                                 // 
	Vector3     m_normalDirection[3];                                                       // 
	float       m_length[3];                                                                // 

	AABB        m_wrapAABB;                                                                 // OBBを取り囲むAABB

}OBB;


bool Intersect(const AABB& _boxA, const AABB& _boxB);                                         // AABB同士の衝突
bool Intersect(const Sphere& _sphere, const AABB& _box);                                // 球体とAABBとの衝突
bool Intersect(const AABB& _box, const Wall& _wall, CollisionInfo& _info_);                 // AABBと壁面の衝突
bool Intersect(const AABB& _box, const Vector3& _point, CollisionInfo& _info_);             // AABBと点の衝突
bool Intersect(const Line& _line, const AABB& _box, CollisionInfo& _info_);                 // 線分とAABBの衝突
bool Intersect(const Wall& _wall, const Line& _line, CollisionInfo& _info_);                 // 壁面と線分の当たり判定

bool IntersectWallEdgeFix(const AABB& _box, const Vector3& _edgeLow, const Vector3& _edgeHi, Vector3& _fixVec_);

// AABBのめり込みを戻す量を計算
void CalcCollisionFixVec(const AABB& _movableBox, const AABB& _fixedBox, Vector3& _calcFixVec_);
// 点と平面との符号付き距離を返す
float PlaneToPointDist(const Vector3& _position, const Plane _plane);