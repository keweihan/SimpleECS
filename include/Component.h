#pragma once
namespace SimpleECS 
{
	// Component class
	class Component {
	public:

		/*
		Called on every frame of rendering
		*/
		virtual void update() = 0;


		/*
		Called on instantiation of this component.
		*/
		virtual void initialize() = 0;

	private:

	};
}
