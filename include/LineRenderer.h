#pragma once
#include <Component.h>
#include <Color.h>
#include <Vector.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	Renders a line with variable thickness from defined startPoint to endPoint. 
	*/
	class SIMPLEECS_API LineRenderer : public Component
	{
	public:
		LineRenderer(Vector startPoint, Vector endPoint, int width, Color color) : 
			start(startPoint), end(endPoint), spacing(0), width(width), renderColor(color) {};

		LineRenderer(Vector startPoint, Vector endPoint, int width, Color color, double spacing) :
			start(startPoint), end(endPoint), spacing(spacing), width(width), renderColor(color) {};

		Vector start, end;
		Color renderColor;
		double spacing;
		int width;

		void update() override;
		void initialize() override {};
	
	private:
		void drawECSLine(Vector startPoint, Vector endPoint);
	};
}
