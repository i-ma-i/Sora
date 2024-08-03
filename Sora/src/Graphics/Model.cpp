#include "Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <external/tiny_obj_loader.h>

#include "AssetRegistry.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

namespace sora
{
	Model::Model(Graphics* graphics, std::filesystem::path path)
		: m_graphics(graphics)
	{
		path.make_preferred();
		// ファイル名を出力する。
		{
			if (std::filesystem::exists(path) == false)
			{
				LOG_ERROR("File not found. path[{}]", path.string());
				__debugbreak();
			}
			LOG_INFO("Read model from obj. path[{}]", path.string());
		}

		const auto device = m_graphics->GetDevice();
		const auto context = m_graphics->GetContext();

		tinyobj::attrib_t inattrib;
		std::vector<tinyobj::shape_t> inshapes;
		std::string warn, err;

		bool result = tinyobj::LoadObj(
			&inattrib, &inshapes, &materials,
			&warn, &err,
			path.string().c_str(), path.parent_path().string().c_str()
		);
		// モデルファイル読み込みによるエラーと警告を出力する。
		{
			if (!warn.empty()) LOG_WARNING("WARN: {}", warn);
			if (!err.empty()) LOG_ERROR("ERR: {}", err);
			if (!result) {
				LOG_ERROR("Failed to load obj.");
				__debugbreak();
			}
		}

		// モデルファイルのメタデータを出力する。
		{
			LOG_DEBUG("Vertex = {}", (int)(inattrib.vertices.size()) / 3);
			LOG_DEBUG("Normal = {}", (int)(inattrib.normals.size()) / 3);
			LOG_DEBUG("UV     = {}", (int)(inattrib.texcoords.size()) / 2);
			LOG_DEBUG("Mat    = {}", (int)materials.size());
			LOG_DEBUG("Shape  = {}", (int)inshapes.size());
		}

		// テクスチャを読み込む。
		for (size_t m = 0; m < materials.size(); m++)
		{
			tinyobj::material_t* mp = &materials[m];

			if (mp->diffuse_texname.length() > 0) {

				if (textures.find(mp->diffuse_texname) == textures.end()) {
					std::filesystem::path texture_filename = mp->diffuse_texname;
					if (std::filesystem::exists(texture_filename) == false)
					{
						texture_filename = path.parent_path() / texture_filename;
						if (std::filesystem::exists(texture_filename) == false)
						{
							LOG_ERROR("Texture not found.  path[{}]", texture_filename.string());
							__debugbreak();
						}
					}

					// テクスチャを読み込む。
					{
						ComPtr<ID3D11ShaderResourceView> texture;
						HRESULT hr = DirectX::CreateWICTextureFromFile(
							device, context,
							texture_filename.c_str(),
							nullptr,
							texture.GetAddressOf()
						);
						if (FAILED(hr))
						{
							LOG_ERROR("Failed to load texture.  path[{}]", texture_filename.string());
							__debugbreak();
						}
						LOG_DEBUG("Texture loaded. [path]{}", texture_filename.string());
						textures[mp->diffuse_texname] = texture;
					}
				}
			}
		}

		std::vector<tinyobj::shape_t>& shapes = inshapes;
		tinyobj::attrib_t& attrib = inattrib;
		for (size_t s = 0; s < shapes.size(); s++)
		{
			DrawObject o;
			std::vector<float> buffer;

			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
			{
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				int current_material_id = shapes[s].mesh.material_ids[f];
				if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size()))) current_material_id = materials.size() - 1;

				float diffuse[3];
				for (int i = 0; i < 3; i++)
				{
					diffuse[i] = materials[current_material_id].diffuse[i];
				}
				float tc[3][2];
				if (attrib.texcoords.size() > 0)
				{
					if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) || (idx2.texcoord_index < 0))
					{
						tc[0][0] = 0.0f;
						tc[0][1] = 0.0f;
						tc[1][0] = 0.0f;
						tc[1][1] = 0.0f;
						tc[2][0] = 0.0f;
						tc[2][1] = 0.0f;
					}
					else
					{
						// Flip Y coord.
						tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
						tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
						tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
						tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
						tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
						tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
					}
				}
				else
				{
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				float v[3][3];
				for (int k = 0; k < 3; k++)
				{
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
				}
				float n[3][3];
				if (attrib.normals.size() > 0) {
					int nf0 = idx0.normal_index;
					int nf1 = idx1.normal_index;
					int nf2 = idx2.normal_index;

					for (int k = 0; k < 3; k++) {
						n[0][k] = attrib.normals[3 * nf0 + k];
						n[1][k] = attrib.normals[3 * nf1 + k];
						n[2][k] = attrib.normals[3 * nf2 + k];
					}
				}

				for (int k = 0; k < 3; k++)
				{
					buffer.push_back(v[k][0]);
					buffer.push_back(v[k][1]);
					buffer.push_back(v[k][2]);
					buffer.push_back(n[k][0]);
					buffer.push_back(n[k][1]);
					buffer.push_back(n[k][2]);
					buffer.push_back(tc[k][0]);
					buffer.push_back(tc[k][1]);
				}
			}

			{
				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(float) * buffer.size();
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA InitData;
				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = buffer.data();

				ComPtr<ID3D11Buffer> vertexBuffer;
				HRESULT hr = device->CreateBuffer(&bd, &InitData, vertexBuffer.GetAddressOf());
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to create vertex buffer.");
					__debugbreak();
				}
				o.vertexBuffer = vertexBuffer;
			}
			o.material_id = shapes[s].mesh.material_ids[0];
			o.numTriangles = buffer.size() / (3 + 3 + 2) / 3;

			drawObjects.push_back(o);
		}
	}

	Model::~Model()
	{
		textures.clear();
		drawObjects.clear();
		materials.clear();
		m_graphics = nullptr;
	}

	void Model::Draw() const
	{
		const auto context = m_graphics->GetContext();

		AssetRegistry::GetAsset<VertexShader>(VS)->SetPipeline();
		AssetRegistry::GetAsset<PixelShader>(PS)->SetPipeline();

		UINT stride = sizeof(DirectX::VertexPositionNormalTexture);
		UINT offset = 0;
		for (auto& o : drawObjects)
		{
			std::string diffuse_texname = materials[o.material_id].diffuse_texname;
			if (diffuse_texname.length() > 0)
			{
				auto it = textures.find(diffuse_texname);
				if (it != textures.end())
				{
					context->PSSetShaderResources(0, 1, it->second.GetAddressOf());
				}
			}
			context->IASetVertexBuffers(0, 1, o.vertexBuffer.GetAddressOf(), &stride, &offset);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->Draw(o.numTriangles * 3, 0);
		}
	}
}
