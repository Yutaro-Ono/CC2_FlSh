#pragma once





class PBRRenderer
{
public:

	PBRRenderer(class Renderer* _renderer);
	~PBRRenderer();

	bool Initialize();




private:

	class CubeMapComponent* m_cubeMap;

};