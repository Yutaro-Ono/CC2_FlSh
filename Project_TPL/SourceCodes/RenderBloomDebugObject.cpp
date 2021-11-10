#include "RenderBloomDebugObject.h"
#include "RenderBloom.h"
#include "GameMain.h"
#include "Debugger.h"


RenderBloomDebugObject::RenderBloomDebugObject(RenderBloom* _target)
	:m_bloom(_target)
{
}

RenderBloomDebugObject::~RenderBloomDebugObject()
{
}

void RenderBloomDebugObject::Update(float _deltaTime)
{

	// ���x��
	std::string label = "Post Process : Bloom";

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		// �I�o�̒���
		float exposureRate = m_bloom->m_exposure;
		ImGui::SliderFloat("Exposure Rate:", &exposureRate, 0.0f, 10.0f);
		if (exposureRate != m_bloom->m_exposure)
		{
			m_bloom->m_exposure = exposureRate;
		}

		// �K���}����
		float gammaRate = m_bloom->m_gamma;
		ImGui::SliderFloat("Gamma Rate:", &gammaRate, 0.0f, 10.0f);
		if (gammaRate != m_bloom->m_gamma)
		{
			m_bloom->m_gamma = gammaRate;
		}

		ImGui::TreePop();
	}
}

void RenderBloomDebugObject::Render()
{
}