#pragma once

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

#include <cstdint>

namespace SimpleECS
{
	/**
	 * Representation of color with rgba values.
	 */
	class SIMPLEECS_API Color {

	public:
		// TODO: Hexadecimal constructor
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {};
		Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(1) {}
		Color() : r(0), g(0), b(0), a(1) {};

		uint8_t r, g, b, a;
	};
}
