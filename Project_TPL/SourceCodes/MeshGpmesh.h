#pragma once
#include "Mesh.h"

class MeshGpmesh : public Mesh
{

public:

	MeshGpmesh();
	~MeshGpmesh();

	// gpmesht@CΜ[h ¨ Έ_zρμ¬ ¨ o^
	bool Load(const std::string& in_filePath) override;

};