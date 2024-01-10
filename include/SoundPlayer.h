#pragma once
#include <Component.h>
#include <Color.h>
#include <string>
#include <memory>


#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	Stores reference to audio file and allows playing audio. 
	*/
	class SoundPlayer : public Component
	{
	public:
		SIMPLEECS_API SoundPlayer(std::string pathToAudio);

		/**
		Plays associated audio clip once.
		*/
		void playAudio();
		std::string path;

		void SIMPLEECS_API initialize() override;
		void SIMPLEECS_API update() override;
	private:
		class SoundPlayerImpl;
		std::unique_ptr<SoundPlayerImpl> pImpl;
	};
}
