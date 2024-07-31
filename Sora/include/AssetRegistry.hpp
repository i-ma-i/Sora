#pragma once
#include "Asset.hpp"

namespace sora
{
	class Graphics;

	class AssetRegistry
	{
	public:
		AssetRegistry(Graphics* graphics);
		~AssetRegistry();

		template<class AssetType, typename ...Args>
		static void Register(std::string_view key, Args&& ...args);

		template<class AssetType>
		[[nodiscard]]
		static auto* GetAsset(std::string_view key) noexcept
		{
			auto it = s_instance->m_registry.find(key.data());
			if(it != s_instance->m_registry.end())
			{
				if(!it->second->IsLoadedInMemory())
					it->second->LoadToMemory();

				return static_cast<AssetType*>(it->second.get());
			}
			__debugbreak();
			return new AssetType();
		}

	private:
		[[nodiscard]]
		inline bool Exists(std::string_view key) const noexcept { return m_registry.find(key.data()) != m_registry.end(); }

	private:
		inline static AssetRegistry* s_instance = nullptr;

		std::filesystem::path m_assetRoot;
		std::unordered_map<std::string, std::unique_ptr<Asset>> m_registry;
	};

	template<class AssetType, typename ...Args>
	inline void AssetRegistry::Register(std::string_view key, Args&& ...args)
	{
		// すでに登録されている場合は何もしない。
		if (s_instance->Exists(key)) return;

		// アセットパスが無効な場合。
		const auto assetPath = s_instance->m_assetRoot / Config::GetString(key);
		if (!std::filesystem::exists(assetPath))
		{
			LOG_ERROR("Asset path is invalid.  path: {}", assetPath.string());
			__debugbreak();
		}

		s_instance->m_registry[key.data()] = std::make_unique<AssetType>(assetPath, std::forward<Args>(args)...);
	}
}
