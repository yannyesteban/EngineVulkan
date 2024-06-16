#pragma once
#include "Entity.h"
namespace SEVIAN {

	class Scene
	{
	private:


	public:
		int status = 0;
		int mode = 0;
		std::vector<Entity3D> entities;
		void init ();
		void render ();
	};

}