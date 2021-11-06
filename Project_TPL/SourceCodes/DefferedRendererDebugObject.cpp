#include "DefferedRendererDebugObject.h"
#include "GameMain.h"
#include "Renderer.h"
#include "DefferedRenderer.h"
#include "Debugger.h"
#include "ShaderManager.h"
#include "VertexArray.h"

DefferedRendererDebugObject::DefferedRendererDebugObject(DefferedRenderer* _defRenderer)
	:m_defRenderer(_defRenderer)
{
}

DefferedRendererDebugObject::~DefferedRendererDebugObject()
{

}

void DefferedRendererDebugObject::Update(float _deltaTime)
{
	// ƒ‰ƒxƒ‹
	std::string label = "DefferedRenderer";

	// ‹æØ‚èü
	ImGui::Separator();

	if (ImGui::TreeNode("GBuffer : POSITION"))
	{
		ImGui::ImageButton((void*)m_defRenderer->m_gPos,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : NORMAL"))
	{
		ImGui::ImageButton((void*)m_defRenderer->m_gNormal,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : ALBEDO"))
	{

		ImGui::ImageButton((void*)m_defRenderer->m_gAlbedoSpec,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Buffer : SSAO"))
	{
		ImGui::ImageButton((void*)m_defRenderer->m_ssaoColor,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : SSAO BLUR"))
	{
		ImGui::ImageButton((void*)m_defRenderer->m_ssaoBlurColor,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : SSAO NOISE"))
	{
		ImGui::ImageButton((void*)m_defRenderer->m_noiseTex,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}
}

void DefferedRendererDebugObject::Render()
{


}
