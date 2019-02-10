#include "CollisionManager.h"

#include <algorithm>
#include <vector>

#include "Root.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Mesh.h"

using namespace Lag;

void CollisionManager::checkCollisions() const {

    std::vector<std::pair<Entity*, Entity*>> colliding;
    std::vector<IntersectionResult> results;

    auto &sm = Root::getInstance().getSceneManager();
    if(sm.hasActiveScene()) {
        auto &entityList = sm.getActiveScene()->getEntities();
        for(auto entity1 : entityList) {
            if(entity1->isCollider()) {
                for(auto entity2 : entityList) {
                    if(entity2->isCollider() && entity1 != entity2) {
                        auto &bv1 = entity1->getWorldSpaceBoundingVolume();
                        auto &bv2 = entity2->getWorldSpaceBoundingVolume();

                        auto pair1 = std::make_pair(entity1, entity2);
                        auto pair2 = std::make_pair(entity2, entity1);

                        auto intersectionResult = bv1.intersects(bv2);

                        if(intersectionResult.intersects && std::find(colliding.begin(), colliding.end(), pair2) == colliding.end()) {
                            colliding.push_back(pair1);
                            results.push_back(intersectionResult);
                        }
                    }
                }
            }
        }

        for(int i = 0; i < colliding.size(); ++i) {
            auto &pair = colliding[i];
            auto &result = results[i];

            onCollisionNotify(*pair.first, *pair.second, result);

            pair.first->onCollision(*pair.second, result);

            auto intersectionResultInv = result;
            intersectionResultInv.penetration = -result.penetration;
            pair.second->onCollision(*pair.first, intersectionResultInv);
        }
    }
}