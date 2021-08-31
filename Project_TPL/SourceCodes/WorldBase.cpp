#include "WorldBase.h"

WorldBase::WorldBase()
	:m_canvas(nullptr)
	,m_environment(nullptr)
{
}

WorldBase::~WorldBase()
{
	delete m_environment;
	delete m_canvas;
}