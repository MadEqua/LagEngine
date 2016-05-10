#include "Root.h"
#include "renderer/SceneGraph.h"
#include "renderer/SceneNode.h"

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

int main()
{
	Root::getInstance().initializeLag("startup.ini");

	SceneGraph &sg = Root::getInstance().getSceneGraph();

	SceneNode &rootNode = sg.getRootSceneNode();
	SceneNode &node1 = rootNode.createChildSceneNode("node1");

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

	print("\n\nmatrix", node1.getFinalTransform());


	Root::getInstance().startRenderingLoop();
}