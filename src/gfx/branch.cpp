// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/branch.h"
#include "gfx/leaves/spawnpoint.h"

Branch::Branch()
{
    leaves_.push_back(std::make_unique<SpawnPoint>());
    leaves_[0]->setTransformationMatrix(leaves_[0]->matrix().translate(50, 0).rotate(-10).scale(0.98, 0.98));
}

void Branch::draw(std::shared_ptr<QPainter> painter, uint num_iterations)
{
    // each invocation of draw() immediately uses up one iteration
    num_iterations--;

    // TODO: add a branch's own proper transformation matrix, so that even the first branch can have all of its elements transformed

    for (auto &leaf : leaves_) {

        if (!leaf->isSpawnPoint()) {
            leaf->draw(painter);

        } else if (num_iterations > 0) {
            leaf->draw(painter);
            draw(painter, num_iterations);
            leaf->unapplyLocalTransformations(painter);
        }

    }
}
