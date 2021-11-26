#pragma once
#include "DebugObjectBase.h"



class TitleUIScreenTPLDebugObject : public DebugObjectBase
{

public:

	TitleUIScreenTPLDebugObject(class TitleUIScreenTPL* _owner);
	~TitleUIScreenTPLDebugObject();

	void Update(float _deltaTime) override;
	void Render() override;

private:

	class TitleUIScreenTPL* m_owner;



};