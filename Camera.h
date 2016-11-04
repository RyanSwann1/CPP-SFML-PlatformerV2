#pragma once

class Entity;
struct SharedContext;
class Camera
{
public:
	Camera(SharedContext* sharedContext);
	~Camera();

	void update(Entity* playera);
	void reset();

private:
	SharedContext* m_sharedContext;
};

