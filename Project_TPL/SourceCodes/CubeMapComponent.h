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

	
	void SetIsVisible(bool _visible) { m_isVisible = _visible; }         // �L���[�u��\�����邩���Ȃ����̃Z�b�g
	void SetIntensity(float _intensity) { m_intensity = _intensity; }    // �P�x���̃Z�b�g


private:

	class Texture* m_texture;

	float m_intensity;               // �P�x���

	bool m_isVisible;                // �`�悷�邩���Ȃ����̃t���O

};