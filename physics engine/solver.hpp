#pragma once
#include "vec2.hpp"
#include "vec2math.hpp"
#include "verletObject.hpp"
#include "link.hpp"
#include <vector>

struct Solver
{
    Vec2 gravity = { 0, 1000 };

    void update(std::vector<VerletObject>& verletObjects, std::vector<Link>& links, float dt)
    {
        const uint32_t sub_steps = 8;
        const float sub_dt = dt / sub_steps;
        for (uint32_t i(sub_steps); i--;) {
            applyGravity(verletObjects);
            applyConstraint(verletObjects);

            for (int link_i = 0; link_i < links.size(); link_i++)
            {
                links[link_i].apply(verletObjects);
            }

            solveCollisions(verletObjects);
            updatePositions(verletObjects, sub_dt);
        }
    }

    void applyGravity(std::vector<VerletObject>& verletObjects)
    {
        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            verletObjects[i].accelerate(gravity);
        }
    }

    void applyConstraint(std::vector<VerletObject>& verletObjects)
    {
        const Vec2 position = { 0, 0 };
        const float radius = 800;

        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            VerletObject& obj = verletObjects[i];

            const Vec2 to_obj = obj.position_current - position;
            const float dist = Vec2Math::length(to_obj);

            if (dist > radius - obj.radius) {
                const Vec2 n = to_obj / dist;
                obj.position_current = position + n * (radius - obj.radius);
            }
        }
    }

    void solveCollisions(std::vector<VerletObject>& verletObjects)
    {
        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            VerletObject& object_1 = verletObjects[i];

            for (int i2 = 0; i2 < verletObjects.size(); i2++)
            {
                VerletObject& object_2 = verletObjects[i2];

                const Vec2 collision_axis = object_1.position_current - object_2.position_current;
                const float dist = Vec2Math::length(collision_axis);
                const float min_dist = object_1.radius + object_2.radius;
                if (dist < min_dist) {
                    const Vec2 n = collision_axis / dist;
                    const float delta = min_dist - dist;
                    if (object_1.physics) object_1.position_current += n * delta * 0.5f;
                    if (object_2.physics) object_2.position_current -= n * delta * 0.5f;
                }
            }
        }
    }

    void updatePositions(std::vector<VerletObject>& verletObjects, float dt)
    {
        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            verletObjects[i].updatePosition(dt);
        }
    }
};