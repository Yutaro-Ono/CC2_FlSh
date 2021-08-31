#pragma once
#include "MeshComponent.h"
#include <string>
#include <vector>
class CubeMapComponent : public Component
{

public:

	CubeMapComponent(class Actor* _owner);
	~CubeMapComponent();

	void CreateTexture(const std::string& _filePath);

	void Draw(class GLSLprogram* _shader);

	class Texture* GetSkyBoxTexture() { return m_texture; }

	
	void SetIsVisible(bool _visible) { m_isVisible = _visible; }         // キューブを表示するかしないかのセット
	void SetIntensity(float _intensity) { m_intensity = _intensity; }    // 輝度情報のセット


private:

	class Texture* m_texture;

	float m_intensity;               // 輝度情報

	bool m_isVisible;                // 描画するかしないかのフラグ

};