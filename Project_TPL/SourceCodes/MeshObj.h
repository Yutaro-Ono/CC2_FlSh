#pragma once
#include "Mesh.h"

class MeshObj : public Mesh
{

public:

	MeshObj();
	~MeshObj();

	// objファイルのロード → 頂点配列作成 → 登録
	bool Load(const std::string& in_filePath) override;


};