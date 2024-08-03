// TODO: いったん3Dモデルを描画するための全ての情報を持たせる。要リファクタリング。
#pragma once
#include "Graphics.hpp"

namespace tinyobj
{
	struct material_t;
}

namespace sora
{
	class Model
	{
		struct DrawObject
		{
			ComPtr<ID3D11Buffer> vertexBuffer;
			int numTriangles = 0;
			size_t material_id = -1;
		};

	public:
		Model(Graphics* graphics, std::filesystem::path path);
		~Model();

		void Draw() const;

	private:
		static constexpr std::string_view VS = "Shader.Basic.VS";
		static constexpr std::string_view PS = "Shader.Basic.PS";
		std::map<std::string, ComPtr<ID3D11ShaderResourceView>> textures;
		std::vector<DrawObject> drawObjects;
		std::vector<tinyobj::material_t> materials;
		Graphics* m_graphics = nullptr;
	};
}
