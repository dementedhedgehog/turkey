#ifndef BRUTE_FORCE_COLLISION_DETECTOR_H
#define BRUTE_FORCE_COLLISION_DETECTOR_H

#include "model/i_collision_detector.h"

 
/*
 * Class 
 */
class BruteForceCollisionDetector : public ICollisionDetector {
 public:
    void detect_collisions(
        const std::list<GameObj*> game_objs, 
        std::list<collision_t> & collisions);
};

#endif
