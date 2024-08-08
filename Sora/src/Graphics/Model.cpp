#include "Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <external/tiny_obj_loader.h>

#include "AssetRegistry.hpp"
#include "Shader.hpp"

namespace sora
{
	Model::Model(Graphics* graphics, std::filesystem::path modelpath)
		: m_graphics(graphics)
	{
		using namespace DirectX;

		modelpath.make_preferred();

		// ファイル名を出力する。
		{
			if (std::filesystem::exists(modelpath) == false)
			{
				LOG_ERROR("File not found. modelpath[{}]", modelpath.string());
				__debugbreak();
			}
			LOG_INFO("Read model from obj. modelpath[{}]", modelpath.string());
		}

		const auto device = m_graphics->GetDevice();
		const auto context = m_graphics->GetContext();

		tinyobj::attrib_t inattrib;
		std::vector<tinyobj::shape_t> inshapes;
		std::string warn, err;

		bool result = tinyobj::LoadObj(
			&inattrib, &inshapes, &materials,
			&warn, &err,
			modelpath.string().c_str(), modelpath.parent_path().string().c_str()
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

			if (mp->diffuse_texname.empty())
				continue;

			if(textures.find(mp->diffuse_texname) != textures.end())
				continue;

			std::filesystem::path texture_filename = mp->diffuse_texname;
			if (std::filesystem::exists(texture_filename) == false)
			{
				texture_filename = modelpath.parent_path() / texture_filename;
				if (std::filesystem::exists(texture_filename) == false)
				{
					LOG_ERROR("Texture not found.  path[{}]", texture_filename.string());
					__debugbreak();
				}
			}

			// テクスチャを読み込む。
			{
				ComPtr<ID3D11ShaderResourceView> texture;
				HRESULT hr = CreateWICTextureFromFile(
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

		std::vector<tinyobj::shape_t>& shapes = inshapes;
		tinyobj::attrib_t& attrib = inattrib;
		for (size_t s = 0; s < shapes.size(); s++)
		{
			DrawObject o;
			std::vector<VertexPositionNormalTexture> buffer;
			buffer.reserve(shapes[s].mesh.indices.size());

			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
			{
				int current_material_id = shapes[s].mesh.material_ids[f];

				float diffuse[3] = { materials[current_material_id].diffuse[0], materials[current_material_id].diffuse[1], materials[current_material_id].diffuse[2] };

				const tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				const tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				const tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				// 頂点。
				XMFLOAT3 v[3]
				{
					{
						attrib.vertices[3 * idx0.vertex_index + 0],
						attrib.vertices[3 * idx0.vertex_index + 1],
						attrib.vertices[3 * idx0.vertex_index + 2]
					},
					{
						attrib.vertices[3 * idx1.vertex_index + 0],
						attrib.vertices[3 * idx1.vertex_index + 1],
						attrib.vertices[3 * idx1.vertex_index + 2]
					},
					{
						attrib.vertices[3 * idx2.vertex_index + 0],
						attrib.vertices[3 * idx2.vertex_index + 1],
						attrib.vertices[3 * idx2.vertex_index + 2]
					}
				};

				// 法線。
				XMFLOAT3 n[3]
				{
					{
						attrib.normals[3 * idx0.normal_index + 0],
						attrib.normals[3 * idx0.normal_index + 1],
						attrib.normals[3 * idx0.normal_index + 2]
					},
					{
						attrib.normals[3 * idx1.normal_index + 0],
						attrib.normals[3 * idx1.normal_index + 1],
						attrib.normals[3 * idx1.normal_index + 2]
					},
					{
						attrib.normals[3 * idx2.normal_index + 0],
						attrib.normals[3 * idx2.normal_index + 1],
						attrib.normals[3 * idx2.normal_index + 2]
					}
				};

				// UV。Y成分は反転させる。
				XMFLOAT2 tc[3]
				{
					{
						attrib.texcoords[2 * idx0.texcoord_index],
						1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1]
					},
					{
						attrib.texcoords[2 * idx1.texcoord_index],
						1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1]
					},
					{
						attrib.texcoords[2 * idx2.texcoord_index],
						1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1]
					}
				};

				// 頂点データをバッファに追加する。
				buffer.emplace_back(v[0], n[0], tc[0]);
				buffer.emplace_back(v[1], n[1], tc[1]);
				buffer.emplace_back(v[2], n[2], tc[2]);
			}

			{
				D3D11_BUFFER_DESC bd = {};
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(VertexPositionNormalTexture) * buffer.size();
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = buffer.data();

				ComPtr<ID3D11Buffer> vertexBuffer;
				HRESULT hr = device->CreateBuffer(&bd, &InitData, o.vertexBuffer.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to create vertex buffer.");
					__debugbreak();
				}
			}
			o.material_id = shapes[s].mesh.material_ids[0];
			o.numTriangles = buffer.size() / 3;

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
