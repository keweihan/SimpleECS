#pragma once
#include <Component.h>
#include <Color.h>
#include <string>
#include <memory>


#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class FontRenderer : public Component
	{
	public:
		SIMPLEECS_API FontRenderer(std::string text, std::string pathToFont);

		std::string text;
		std::string path;

		void SIMPLEECS_API initialize() override;
		void SIMPLEECS_API update() override;
	private:
		class FontRendererImpl;
		std::unique_ptr<FontRendererImpl> pImpl;
	};
}
