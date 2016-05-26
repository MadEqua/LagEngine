#include "Root.h"
#include "renderer/RenderWindow.h"
#include "scene/SceneManager.h"
#include "scene/SceneGraph.h"
#include "scene/SceneNode.h"
#include "scene/Camera.h"
#include "scene/Entity.h"
#include "io/InputManager.h"

#include "io/IKeyboardListener.h"
#include "io/ICursorListener.h"
#include "IFrameListener.h"

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

#include "io/Keys.h"

#include <iostream>

using namespace Lag;
using namespace std;

void print(char* s, const glm::vec3 &v)
{
	cout << s << ": " << v[0] << ", " << v[1] << ", " << v[2] << endl;
}

void print(char* s, const glm::vec4 &v)
{
	cout << s << ": " << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << endl;
}

void print(char* s, const glm::quat &q)
{
	glm::vec3 axis = glm::axis(q);
	cout << s << ": " << axis[0] << ", " << axis[1] << ", " << axis[2] << ", " << glm::degrees(glm::angle(q)) << endl;
}

void print(char* s, const glm::mat4 &m)
{
	cout << s << ": ";
	print("col1", m[0]);
	print("col2", m[1]);
	print("col3", m[2]);
	print("col4", m[3]);
}

class InputListener : public IKeyboardListener, public ICursorListener
{
public:
	InputListener(Camera &camera) : camera(camera), lastX(-1), lastY(-1) {}

	const float MOVE_VELOCITY = 0.15f;
	
	virtual void onKeyPress(int key, int modifier)
	{
		treatKey(key);
	}

	virtual void onKeyRelease(int key, int modifier) 
	{
	}

	virtual void onKeyRepeat(int key, int modifier)
	{
		treatKey(key);
	}

	virtual void onCursorMove(int x, int y)
	{
		int dx, dy;

		if (lastX != -1)
		{
			dx = lastX - x;
			camera.getParentSceneNode()->yaw(dx * 0.1f, WORLD);
		}

		if (lastY != -1) 
		{
			dy = lastY - y;
			camera.getParentSceneNode()->pitch(dy * 0.1f, LOCAL);
		}
		
		lastX = x;
		lastY = y;
	}

	virtual void onButtonPressed(int x, int y, int button, int modifiers)
	{

	}

	virtual void onButtonReleased(int x, int y, int button, int modifiers)
	{

	}

	void treatKey(int key)
	{
		switch (key)
		{
		case KEY_W:
			camera.getParentSceneNode()->translate(glm::vec3(0, 0, -MOVE_VELOCITY), LOCAL);
			break;
		case KEY_S:
			camera.getParentSceneNode()->translate(glm::vec3(0, 0, MOVE_VELOCITY), LOCAL);
			break;
		case KEY_A:
			camera.getParentSceneNode()->translate(glm::vec3(-MOVE_VELOCITY, 0, 0), LOCAL);
			break;
		case KEY_D:
			camera.getParentSceneNode()->translate(glm::vec3(MOVE_VELOCITY, 0, 0), LOCAL);
			break;
		default:
			break;
		}
	}
	int lastX, lastY;
	Camera &camera;
};

class FrameListener : public IFrameListener
{
	virtual void onFrameStart(float timePassed)
	{
		///cout << "Frame Start\n";
	}

	virtual void onFrameRenderingQueued(float timePassed)
	{
	}

	virtual void onFrameEnd(float timePassed)
	{
	}
};

int main()
{
	if(!Root::getInstance().initializeLag("startup.ini"))
		return 1;

	RenderWindow &renderWindow = Root::getInstance().getRenderWindow();
	SceneManager &sm = Root::getInstance().getSceneManager();
	
	FrameListener fl;

	Root::getInstance().registerObserver(fl);

	SceneNode &rootNode = sm.getSceneGraph().getRootSceneNode();
	SceneNode &childNode = rootNode.createChildSceneNode("child");
	SceneNode &cameraNode = rootNode.createChildSceneNode("cameraNode");

	Camera &cam = sm.createCamera("camera", 45.0f, 0.1f, 100.0f);
	renderWindow.createViewport("default", cam);
	cam.attachToSceneNode(cameraNode);

	InputListener il(cam);
	Root::getInstance().getInputManager().registerObserver(static_cast<IKeyboardListener&>(il));
	Root::getInstance().getInputManager().registerObserver(static_cast<ICursorListener&>(il));

	Entity *ent = sm.createEntity("sphere", "sphere", "testMaterial");
	ent->attachToSceneNode(childNode);

	//Entity *ent2 = sm.createEntity("sphere", "sphere", "testMaterial");
	//ent2->attachToSceneNode(childNode);

	//rootNode.scale(glm::vec3(0.3f));
	//childNode.scale(glm::vec3(0.1f));
	//childNode.translate(glm::vec3(0,0,0), WORLD);

	cameraNode.translate(glm::vec3(0, 0, 10.0f), WORLD);
	//cameraNode.yaw(-10, WORLD);
	//cameraNode.pitch(-10, WORLD);

	Root::getInstance().startRenderingLoop();
	return 0;
}