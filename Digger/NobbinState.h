#pragma once
namespace dae 
{
	class NobbinControllerComponent;

	class NobbinState
	{
	public:
		virtual ~NobbinState() = default;

		virtual void OnEnter(NobbinControllerComponent& controller) = 0;
		virtual void Update(NobbinControllerComponent& controller, float deltaTime) = 0;
		virtual void OnExit(NobbinControllerComponent& controller) = 0;
	};
}