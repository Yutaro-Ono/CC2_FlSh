#pragma once
#include "Mesh.h"

class MeshObj : public Mesh
{

public:

	MeshObj();
	~MeshObj();

	// objt@CÌ[h ¨ ¸_zñì¬ ¨ o^
	bool Load(const std::string& in_filePath) override;


};