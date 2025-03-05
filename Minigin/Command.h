#pragma once
#include <iostream>
namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

    class JumpCommand : public Command
    {
    public:
        void Execute() override
        {
            std::cout << "Jump!\n";
        }
    };

    class FireCommand : public Command
    {
    public:
        void Execute() override
        {
            std::cout << "Fire!\n";
        }
    };
}

