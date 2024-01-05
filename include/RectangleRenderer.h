#pragma once
#include <SimpleECS_Core.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class RectangleRenderer : public Component
	{
	public:
		int width, height;
		void SIMPLEECS_API update() override;
		void SIMPLEECS_API initialize() override;
	};
}
