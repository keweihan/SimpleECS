#pragma once
#include <SimpleECS_Core.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	* Representation of a 2D vector. Contains methods for common vector operations.
	*/
	class SIMPLEECS_API Vector
	{
	public:
		Vector() : x(0), y(0) {}
		Vector(double x, double y) : x(x), y(y) {}

		double x, y;
	};
}
