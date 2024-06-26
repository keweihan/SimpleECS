#pragma once
#ifndef TIME_H
#define TIME_H

#include <cstdint>

#include "SimpleECSAPI.h"

namespace SimpleECS
{
	class Game;

	/**
	Provides time related information
	*/
	class SIMPLEECS_API Timer {
	public:
		// Game has access to endFrame()
		friend class Game;

		/*
		* Milliseconds since last frame execution finish 
		* (frame 'finish' time excludes time to render).
		*/
		static uint64_t getDeltaTime();

		/*
		* Milliseconds since program started
		*/
		static uint64_t getProgramLifetime();

	private:
		// Time of previous frame finish in ms
		static uint64_t frameFinishTime;

		// Duration of previous frame
		static uint16_t previousFrameLength; 

		/*
		* Call at end of every game loop to indicate end
		*/
		static void endFrame();
	};
}

#endif
