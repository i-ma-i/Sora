#include "Engine.hpp"

#include "CKeyboard.hpp"
#include "CMouse.hpp"

namespace sora
{
	Engine::Engine(): m_modules
	(
		std::make_unique<CKeyboard>(),
		std::make_unique<CMouse>()
	)
	{
		s_engine = this;
	}

	Engine::~Engine()
	{
		s_engine = nullptr;
	}
}
