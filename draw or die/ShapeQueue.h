#pragma once
#include "Resources.h"
class ShapeQueue
{
	deque<Sprite> m_sprites;
	friend class Application;
};

