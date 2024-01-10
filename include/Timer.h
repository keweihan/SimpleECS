#pragma once
#ifndef TIME_H
#define TIME_H

#include <cstdint>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

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
		static int64_t getDeltaTime();

		/*
		* Milliseconds since program started
		*/
		static int64_t getProgramLifetime();

	private:
		// Time of previous frame finish in ms
		static int64_t frameFinishTime;

		/*
		* Call at end of every game loop to indicate end
		*/
		static void endFrame();
	};
}

#endif
