#pragma once
#include <tuple>

namespace sora
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		template<class Module>
		[[nodiscard]]
		static auto& GetModule() noexcept
		{
			return *(std::get<std::unique_ptr<Module>>(s_engine->m_modules).get());
		}

	private:
		inline static Engine* s_engine = nullptr;

		std::tuple<
			std::unique_ptr<class IKeyboard>,
			std::unique_ptr<class IMouse>
		> m_modules;
	};
}
