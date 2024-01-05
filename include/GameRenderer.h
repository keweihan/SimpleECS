#pragma once
#include <memory>

namespace SimpleECS
{
	class GameRenderer {
	public:
		/**
		 * Initializes game window and renderer
		 */
		static void initGameRenderer();
	protected:
		/**
		 * Forward declaration to hide SDL implementation details.
		 * Note: introduces minor performance tradeoffs. See
		 * https://en.cppreference.com/w/cpp/language/pimpl
		 */
		class GameRendererImpl;
		static std::unique_ptr<GameRendererImpl> pImpl;
	};
}