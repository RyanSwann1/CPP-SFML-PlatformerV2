#pragma once
#include "EntityState.h"
#include <array>
#include <string>
namespace 
{
	//http://stackoverflow.com/questions/6469849/one-or-more-multiply-defined-symbols-found
	//https://www.youtube.com/watch?v=p33yOjgPSb4
	extern std::string name = "Bob";
	struct AnimationNames
	{
		std::array<std::string, static_cast<size_t>(EntityState::Count)> animationNames;
	};
	//struct Names
	//{
	//	extern costd::array<std::string, static_cast<size_t>(EntityState::Count)> m_animationNames;
	//};
	
}

