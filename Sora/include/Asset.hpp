#pragma once

namespace sora
{
	class Asset
	{
	public:
		Asset() = default;
		Asset(const std::filesystem::path& path) : m_path(path) {}
		virtual ~Asset() = default;

		virtual void LoadToMemory() = 0;

		bool IsLoadedInMemory() const noexcept { return m_loadedInMemory; }

	protected:
		std::filesystem::path m_path;
		bool m_loadedInMemory = false;
	};
}
