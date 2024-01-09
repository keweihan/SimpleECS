#pragma once
#include "Vector.h"

namespace UtilSimpleECS {
	class TransformUtil {
	public:
		/**
		 * Converts from a world space coordinate (x, y) to SDL surface coordinate.
		 * 
		 * @param (x,y) pair representing a world space coordinate used by Transforms
		 * @returns (x,y) pair representing a pixel location in SDL surface coordinates.
		 */
		static SimpleECS::Vector  worldToScreenSpace(double x, double y);

		/**
		 * Converts from a world space coordinate (x, y) to SDL surface coordinate.
		 *
		 * @param (x,y) pair representing a pixel location in SDL surface coordinates.
		 * @returns (x,y) pair representing a world space coordinate used by Transforms
		 */
		static SimpleECS::Vector  screenToWorldSpace(int x, int y);
	};
}