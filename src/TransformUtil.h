#pragma once
#include <tuple>

namespace UtilSimpleECS {
	class TransformUtil {
	public:
		/**
		 * Converts from a world space coordinate (x, y) to SDL surface coordinate.
		 * 
		 * @param (x,y) pair representing a world space coordinate used by Transforms
		 * @returns (x,y) pair representing a pixel location in SDL surface coordinates.
		 */
		static std::pair<int, int> worldToScreenSpace(int x, int y);

		/**
		 * Converts from a world space coordinate (x, y) to SDL surface coordinate.
		 *
		 * @param (x,y) pair representing a pixel location in SDL surface coordinates.
		 * @returns (x,y) pair representing a world space coordinate used by Transforms
		 */
		static std::pair<int, int> screenToWorldSpace(int x, int y);
	};
}