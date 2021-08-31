//-----------------------------------------------------------------------+
//
// Canvas�N���X (UI�N���X�����[���h�N���X���Q�Ƃł���悤�ɂ���)
//
//-----------------------------------------------------------------------+
#pragma once
#include <vector>

class Canvas
{
public:


	Canvas(class WorldBase* in_world);     // �R���X�g���N�^
	~Canvas();                             // �f�X�g���N�^

	virtual bool Load() = 0;

	virtual void Update(float _deltaTime);

	void AddUI(class UIScreen* in_ui) { m_interfaces.push_back(in_ui); }


private:


	class WorldBase* m_world;                       // �Q�[�����[���h

	std::vector<class UIScreen*> m_interfaces;      // �C���^�[�t�F�[�X

};