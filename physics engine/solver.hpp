#pragma once
#include "vec2.hpp"
#include "vec2math.hpp"
#include "verletObject.hpp"
#include "link.hpp"
#include "raylib.h"
#include <vector>

struct Solver
{
    const Vec2 gravity = { 0, 1000 };

    void update(std::vector<VerletObject>& verletObjects, std::vector<Link>& links, float dt)
    {
        const uint32_t sub_steps = 8;
        const float sub_dt = dt / sub_steps;

        for (uint32_t i(sub_steps); i--;)
        {
            applyGravity(verletObjects, true);
            // applyConstraint(verletObjects);

            for (int l_i(links.size()); l_i--;) {
                links[l_i].apply(verletObjects);
            }

            solveCollisions(verletObjects, sub_dt);
            // updatePositions(verletObjects, sub_dt);
        }
    }

    void applyGravity(std::vector<VerletObject>& verletObjects, bool constraint = false)
    {
        const Vec2 position = {};
        const float radius = 800;

        for (int i(verletObjects.size()); i--;)
        {
            VerletObject& obj = verletObjects[i];

            if (!obj.physics)
                continue;

            obj.accelerate(gravity);

            if (!constraint)
                continue;

            const Vec2 to_obj = obj.position_current - position;
            const float dist = Vec2Math::length(to_obj);

            if (dist > radius - obj.radius) {
                const Vec2 n = to_obj / dist;
                obj.position_current = position + n * (radius - obj.radius);
            }
        }
    }

    void applyConstraint(std::vector<VerletObject>& verletObjects)
    {
        const Vec2 position = { 0, 0 };
        const float radius = 800;

        for (int i(verletObjects.size()); i--;)
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

    void solveCollisions(std::vector<VerletObject>& verletObjects, float dt)
    {
        for (int i(verletObjects.size()); i--;)
        {
            VerletObject& obj_1 = verletObjects[i];

            if (!obj_1.physics)
                continue;

            for (int i_2(verletObjects.size()); i_2--;)
            {
                VerletObject& obj_2 = verletObjects[i_2];

                const Vec2 collision_axis = obj_1.position_current - obj_2.position_current;
                const float dist = Vec2Math::length(collision_axis);
                const float min_dist = obj_1.radius + obj_2.radius;

                if (dist < min_dist) {
                    const Vec2 n = collision_axis / dist;
                    const float delta = min_dist - dist;

                    if (obj_1.physics) obj_1.position_current += n * delta * 0.5f;
                    if (obj_2.physics) obj_2.position_current -= n * delta * 0.5f;
                }
            }

            verletObjects[i].updatePosition(dt);
        }
    }

    void updatePositions(std::vector<VerletObject>& verletObjects, float dt)
    {
        for (int i(verletObjects.size()); i--;)
        {
            if (!verletObjects[i].physics)
                continue;

            verletObjects[i].updatePosition(dt);
        }
    }
};