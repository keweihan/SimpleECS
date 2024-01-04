#pragma once
namespace SimpleECS 
{
	// Component class
	class Component {
	public:

		/*
		Called on every frame of rendering
		*/
		virtual void Update();


		/*
		Called on instantiation of this component.
		*/
		virtual void Initialize();


	private:

	};
}
