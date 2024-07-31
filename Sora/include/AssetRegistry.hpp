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

		[[nodiscard]]

		std::string GetString(std::string_view key);

	private:
		inline static AssetRegistry* s_instance = nullptr;

		std::filesystem::path m_assetRoot;
		std::unordered_map<std::string, std::unique_ptr<Asset>> m_registry;
	};

	template<class AssetType, typename ...Args>
	inline void AssetRegistry::Register(std::string_view key, Args&& ...args)
	{
		if (s_instance->Exists(key)) return;

		s_instance->m_registry[key.data()] = std::make_unique<AssetType>(s_instance->m_assetRoot / s_instance->GetString(key), std::forward<Args>(args)...);
	}
}
