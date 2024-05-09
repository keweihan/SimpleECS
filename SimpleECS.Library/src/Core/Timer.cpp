#include "Core/Timer.h"
#include <SDL.h>

uint64_t SimpleECS::Timer::frameFinishTime;
uint16_t SimpleECS::Timer::previousFrameLength;

uint64_t SimpleECS::Timer::getDeltaTime()
{
	return previousFrameLength;
}

uint64_t SimpleECS::Timer::getProgramLifetime()
{
	return SDL_GetTicks64();
}

void SimpleECS::Timer::endFrame()
{
	uint64_t currTicks = SDL_GetTicks64();
	previousFrameLength = static_cast<uint16_t>(currTicks - frameFinishTime);

	frameFinishTime = currTicks;
}
