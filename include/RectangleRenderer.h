#pragma once
#include <Component.h>
#include <Color.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	Provides rectangle rendering to an entity. 
	*/
	class SIMPLEECS_API RectangleRenderer : public Component
	{
	public:
		RectangleRenderer() : width(40), height(40), renderColor(Color()) {}
		RectangleRenderer(int w, int h) : width(w), height(h) {}
		RectangleRenderer(int w, int h, Color color) : width(w), height(h), renderColor(color){}

		int width, height;
		Color renderColor;

		void update() override;
		void initialize() override; 
	};
}
