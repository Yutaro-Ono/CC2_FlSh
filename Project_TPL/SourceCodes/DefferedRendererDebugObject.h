#pragma once
#include "DebugObjectBase.h"

class DefferedRendererDebugObject : public DebugObjectBase
{

public:

	DefferedRendererDebugObject(class DefferedRenderer* _defRenderer);
	~DefferedRendererDebugObject();


	void Update(float _deltaTime) override;
	void Render() override;


private:


	class DefferedRenderer* m_defRenderer;


};