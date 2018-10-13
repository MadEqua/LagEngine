#include "CollisionManager.h"

#include <algorithm>
#include <vector>

#include "Root.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Mesh.h"
#include "ICollider.h"

using namespace Lag;

void CollisionManager::checkCollisions() {

    std::vector<ICollider*> colliding;

    auto &sm = Root::getInstance().getSceneManager();
    if(sm.hasActiveScene()) {
        auto &entityList = sm.getActiveScene()->getEntities();
        for(auto entity1 : entityList) {
            if(std::find(colliding.begin(), colliding.end(), entity1) == colliding.end()) {
                for(auto entity2 : entityList) {
                    if(entity1 != entity2) {
                        auto &aabb1 = entity1->getWorldSpaceAABB();
                        auto &aabb2 = entity2->getWorldSpaceAABB();
                        if(aabb1.intersects(aabb2)) {
                            colliding.push_back(dynamic_cast<ICollider*>(entity1));
                            colliding.push_back(dynamic_cast<ICollider*>(entity2));
                        }
                    }
                }
            }
        }

        for(auto collider : colliding) {
            collider->onCollision();
        }
    }
}