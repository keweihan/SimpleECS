#pragma once
#include <SimpleECS_Core.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class SIMPLEECS_API RectangleRenderer : public Component
	{
	public:
		RectangleRenderer() : width(40), height(40) {}
		RectangleRenderer(int w, int h) : width(w), height(h) {}

		int width, height;
		void update() override;
		void initialize() override; 
	};
}
