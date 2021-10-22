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
	// ���x��
	std::string label = "DefferedRenderer";

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode("GBuffer : NORMAL"))
	{

		//// �`�揈�� ------------------------------------------------------------+
		//glDisable(GL_DEPTH_TEST);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//RENDERER->GetShaderManager()->EnableShaderProgram(GLSL_SHADER::OUT_SCREEN);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_defRenderer->m_gNormal);
		////----------------------------------------------------------------------+
		//RENDERER->GetScreenVAO()->SetActive();
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui��ɕ\��
		ImGui::Image((void*)m_defRenderer->m_gNormal,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : ALBEDO"))
	{

		//// �`�揈�� ------------------------------------------------------------+
		//glDisable(GL_DEPTH_TEST);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//RENDERER->GetShaderManager()->EnableShaderProgram(GLSL_SHADER::OUT_SCREEN);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_defRenderer->m_gAlbedoSpec);
		////----------------------------------------------------------------------+
		//RENDERER->GetScreenVAO()->SetActive();
		//glDrawArrays(GL_TRIANGLES, 0, 6);


		// ImGui��ɕ\��
		ImGui::Image((void*)m_defRenderer->m_gAlbedoSpec,
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}
}

void DefferedRendererDebugObject::Render()
{


}
