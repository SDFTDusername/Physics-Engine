#pragma once

#include "raylib.h"
#include "raymath.h"
#include "vector2_math.hpp"
#include "verlet_object.hpp"
#include "link.hpp"
#include <vector>

struct Solver
{
    const Vector2 gravity = { 0, 1000 };

    void update(std::vector<VerletObject>& verletObjects, std::vector<Link>& links, const float dt)
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

            updateObjects(verletObjects, sub_dt);
        }
    }

    void applyGravity(std::vector<VerletObject>& verletObjects, const bool constraint)
    {
        const Vector2 position = {};
        const float radius = 800;

        for (int i(verletObjects.size()); i--;)
        {
            VerletObject& obj = verletObjects[i];

            if (obj.static_object)
                continue;

            obj.accelerate(gravity);

            if (!constraint)
                continue;

            const Vector2 to_obj = obj.position_current - position;
            const float dist = Vector2Length(to_obj);

            if (dist > radius - obj.radius) {
                const Vector2 n = to_obj / dist;
                obj.position_current = position + n * (radius - obj.radius);
            }
        }
    }

    void applyConstraint(std::vector<VerletObject>& verletObjects)
    {
        const Vector2 position = { 0, 0 };
        const float radius = 800;

        for (int i(verletObjects.size()); i--;)
        {
            VerletObject& obj = verletObjects[i];

            if (obj.static_object)
                continue;

            const Vector2 to_obj = obj.position_current - position;
            const float dist = Vector2Length(to_obj);

            if (dist > radius - obj.radius) {
                const Vector2 n = to_obj / dist;
                obj.position_current = position + n * (radius - obj.radius);
            }
        }
    }

    void updateObjects(std::vector<VerletObject>& verletObjects, const float dt)
    {
        for (int i(verletObjects.size()); i--;)
        {
            VerletObject& obj_1 = verletObjects[i];

            if (obj_1.static_object)
                continue;

            for (int i_2(verletObjects.size()); i_2--;)
            {
                VerletObject& obj_2 = verletObjects[i_2];

                const Vector2 collision_axis = obj_1.position_current - obj_2.position_current;
                const float dist = Vector2Length(collision_axis);
                const float min_dist = obj_1.radius + obj_2.radius;

                if (dist < min_dist) {
                    const Vector2 n = collision_axis / dist;
                    const float delta = min_dist - dist;

                    obj_1.position_current += n * delta * 0.5f;
                    if (!obj_2.static_object) obj_2.position_current -= n * delta * 0.5f;
                }
            }

            obj_1.updatePosition(dt);
        }
    }
};