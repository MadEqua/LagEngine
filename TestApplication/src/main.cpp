#include "Root.h"
#include "renderer/RenderWindow.h"
#include "scene/SceneManager.h"
#include "scene/SceneGraph.h"
#include "scene/SceneNode.h"
#include "scene/Camera.h"
#include "io/InputManager.h"

#include "io/IKeyboardListener.h"
#include "io/ICursorListener.h"
#include "IFrameListener.h"

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

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

class InputListener : public IKeyboardListener
{
	virtual void onKeyPress(int key, int modifier)
	{
		cout << "KEY PRESS: " << key << endl;
	}

	virtual void onKeyRelease(int key, int modifier) 
	{
	}

	virtual void onKeyRepeat(int key, int modifier)
	{
	}
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

	SceneNode &rootNode = sm.getSceneGraph().getRootSceneNode();
	/*SceneNode &node1 = rootNode.createChildSceneNode("node1");

	rootNode.roll(90.0f, WORLD);

	node1.translate(glm::vec3(10, 0, 0), PARENT);
	node1.yaw(90, LOCAL);
	
	print("l position", node1.getLocalPosition());
	print("l scale", node1.getLocalScale());
	print("l orientation", node1.getLocalOrientation());

	cout << endl;

	print("w position", node1.getWorldPosition());
	print("w scale", node1.getWorldScale());
	print("w orientation", node1.getWorldOrientation());

	print("\n\nmatrix", node1.getFinalTransform());*/

	FrameListener fl;
	InputListener il;

	Root::getInstance().registerObserver(fl);
	Root::getInstance().getInputManager().registerObserver(il);

	Camera &cam = sm.createCamera("camera");
	cam.attachToSceneNode(rootNode);
	renderWindow.createViewport("default", cam);


	Root::getInstance().startRenderingLoop();
	return 0;
}