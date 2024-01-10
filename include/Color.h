#pragma once

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	 * Representation of color with rgba values.
	 */
	class SIMPLEECS_API Color {

	public:
		// TODO: Hexadecimal constructor
		Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {};
		Color(int r, int g, int b) : r(r), g(g), b(b), a(1) {}
		Color() : r(0), g(0), b(0), a(1) {};

		int r, g, b, a;
	};
}
