#include "Timer.h"
#include <SDL.h>

int64_t SimpleECS::Timer::frameFinishTime;

int64_t SimpleECS::Timer::getDeltaTime()
{
	return SDL_GetTicks64() - frameFinishTime;
}

int64_t SimpleECS::Timer::getProgramLifetime()
{
	return SDL_GetTicks64();
}

void SimpleECS::Timer::endFrame()
{
	frameFinishTime = SDL_GetTicks64();
}
