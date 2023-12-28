// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/branch.h"
#include "gfx/leaves/circle.h"
#include "gfx/leaves/spawnpoint.h"

Branch::Branch()
{
    // each and every branch has at least one spawn (branching) point
    leaves_.push_back(std::make_unique<SpawnPoint>());
    leaves_[0]->setTransformationMatrix(leaves_[0]->matrix().translate(60, 0).rotate(-10).scale(0.98, 0.98));

    // some hardcoded leaves for now:
    leaves_.push_back(std::make_unique<Circle>(15, Qt::green));
    leaves_[1]->setTransformationMatrix(leaves_[1]->matrix().translate(-10, -10));

    leaves_.push_back(std::make_unique<Circle>(20, Qt::red));
    leaves_[2]->setTransformationMatrix(leaves_[2]->matrix().scale(1, 0.5).translate(0, 50));

    leaves_.push_back(std::make_unique<Circle>(30, Qt::blue));
    leaves_[3]->setTransformationMatrix(leaves_[3]->matrix().rotate(30).scale(1, 0.2).translate(60, 0));
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
