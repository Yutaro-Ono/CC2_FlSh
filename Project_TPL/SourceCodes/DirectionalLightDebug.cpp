#include "DirectionalLightDebug.h"
#include "DirectionalLight.h"
#include <string>

DirectionalLightDebug::DirectionalLightDebug(DirectionalLight* _owner)
	:m_owner(_owner)
	,m_diffuseColorPick(false)
	,m_specularColorPick(false)
	,m_ambientColorPick(false)
{
}

DirectionalLightDebug::~DirectionalLightDebug()
{
}

void DirectionalLightDebug::Update(float _deltaTime)
{
	// ラベル
	std::string label = "Directional Light";

	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		//------------------------------------------------------------------------+
		// Diffuse
		ImGui::Text("Directional Light : Diffuse");
		//glm::vec3 color;
		static float diffuse[3];
		diffuse[0] = m_owner->m_diffuse.x;
		diffuse[1] = m_owner->m_diffuse.y;
		diffuse[2] = m_owner->m_diffuse.z;

		label = "Directional Light Color : R";
		ImGui::SliderFloat(label.c_str(), &diffuse[0], 0.0f, 1.0f);
		label = "Directional Light Color : G";
		ImGui::SliderFloat(label.c_str(), &diffuse[1], 0.0f, 1.0f);
		label = "Directional Light Color : B";
		ImGui::SliderFloat(label.c_str(), &diffuse[2], 0.0f, 1.0f);

		// カラーピッカーを使用するか
		ImGui::Checkbox("DiffuseColor Picker", &m_diffuseColorPick);
		if (m_diffuseColorPick)
		{
			ImGui::ColorPicker3("Diffuse Color Picker", diffuse);
		}

		// 更新されたら
		if (diffuse[0] != m_owner->m_diffuse.x || diffuse[1] != m_owner->m_diffuse.y || diffuse[2] != m_owner->m_diffuse.z)
		{
			m_owner->SetDiffuse(Vector3(diffuse[0], diffuse[1], diffuse[2]));
		}
		//--------------------------------------------------------------------------+
		// Specular
		ImGui::Text("Directional Light : Specular");
		//glm::vec3 color;
		static float specular[3];
		specular[0] = m_owner->m_specular.x;
		specular[1] = m_owner->m_specular.y;
		specular[2] = m_owner->m_specular.z;

		label = "Specular : R";
		ImGui::SliderFloat(label.c_str(), &specular[0], 0.0f, 1.0f);
		label = "Specular : G";
		ImGui::SliderFloat(label.c_str(), &specular[1], 0.0f, 1.0f);
		label = "Specular : B";
		ImGui::SliderFloat(label.c_str(), &specular[2], 0.0f, 1.0f);

		// カラーピッカーを使用するか
		ImGui::Checkbox("SpecularColor Picker", &m_specularColorPick);
		if (m_specularColorPick)
		{
			ImGui::ColorPicker3("Specular Color Picker", specular);
		}

		// 更新されたら
		if (specular[0] != m_owner->m_specular.x || specular[1] != m_owner->m_specular.y || specular[2] != m_owner->m_specular.z)
		{
			m_owner->SetSpecular(Vector3(specular[0], specular[1], specular[2]));
		}
		//--------------------------------------------------------------------------+
		// ambient
		ImGui::Text("Directional Light : Ambient");
		//glm::vec3 color;
		static float ambient[3];
		ambient[0] = m_owner->m_ambient.x;
		ambient[1] = m_owner->m_ambient.y;
		ambient[2] = m_owner->m_ambient.z;

		label = "Ambient : R";
		ImGui::SliderFloat(label.c_str(), &ambient[0], 0.0f, 1.0f);
		label = "Ambient : G";
		ImGui::SliderFloat(label.c_str(), &ambient[1], 0.0f, 1.0f);
		label = "Ambient : B";
		ImGui::SliderFloat(label.c_str(), &ambient[2], 0.0f, 1.0f);

		// カラーピッカーを使用するか
		ImGui::Checkbox("AmbientColor Picker", &m_ambientColorPick);
		if (m_ambientColorPick)
		{
			ImGui::ColorPicker3("AmbientColor Picker", ambient);
		}

		// 更新されたら
		if (ambient[0] != m_owner->m_ambient.x || ambient[1] != m_owner->m_ambient.y || ambient[2] != m_owner->m_ambient.z)
		{
			m_owner->SetAmbient(Vector3(ambient[0], ambient[1], ambient[2]));
		}
		//--------------------------------------------------------------------------+

		ImGui::TreePop();
	}
}

void DirectionalLightDebug::Render()
{
}
