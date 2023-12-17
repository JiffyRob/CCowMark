#include "cowmark.h"
#include <SDL2/SDL.h>

CM_Manager *CM_CreateManager(const int max_particles) {
    CM_Manager *manager = malloc(sizeof(CM_Manager));
    manager->particles = malloc(sizeof(CM_Particle) * max_particles);
    manager->source_rects = malloc(sizeof(SDL_Rect) * max_particles);
    manager->dest_rects = malloc(sizeof(SDL_Rect) * max_particles);
    manager->max_particles = max_particles;
    manager->particle_count = 0;
    manager->first_free = 0;
    manager->first_used = -1;
    // Get the particle-particle links and particle-rect links connected
    for (int i = 0; i < max_particles - 1; ++i) {
        manager->particles[i].next = i + 1;
        manager->particles[i].last = i - 1;
        manager->particles[i].dest_rect = &manager->dest_rects[i];
        manager->particles[i].source_rect = &manager->source_rects[i];
    }
    // The last one is -1 for its next free, a list terminator value
    manager->particles[max_particles-1].next = -1;
    return manager;
}

// TODO: Reduce param count here?
int CM_ManagerAddParticle(
    CM_Manager *manager,
    SDL_Texture *texture,
    SDL_Rect *dest_rect,
    SDL_Rect *source_rect,
    double width_vel,
    double height_vel,
    double x_vel,
    double y_vel,
    double life) {

    int new_index = manager->first_free;
    // No free particles
    if (new_index < 0) {
        return -1;
    }
    // Move the particle from the dead list to the live list
    manager->first_free = manager->particles[new_index].next;
    manager->particles[new_index].last = -1;

    manager->particles[new_index].next = manager->first_used;
    manager->particles[manager->particles[new_index].next].last = new_index;

    manager->first_used = new_index;
    manager->particles[new_index].last = -1;
    // Fill in the particle data
    manager->particles[new_index].texture = texture;
    manager->particles[new_index].x = dest_rect->x;
    manager->particles[new_index].y = dest_rect->y;
    manager->particles[new_index].width_vel = width_vel;
    manager->particles[new_index].height_vel = height_vel;
    manager->particles[new_index].x_vel = x_vel;
    manager->particles[new_index].y_vel = y_vel;
    manager->particles[new_index].life = life;
    if (source_rect == NULL) {
        manager->particles[new_index].source_rect = NULL;
    } else {
        manager->particles[new_index].source_rect = manager->source_rects + new_index;
        manager->source_rects[new_index] = *source_rect;
    }

    manager->dest_rects[new_index] = *dest_rect;
    return new_index;
}

int CM_ManagerKillParticle(CM_Manager *manager, int particle_id) {
    // Move it from the live list to the dead list (or put it at the beginning of the dead list if already dead)
    manager->particles[manager->particles[particle_id].last].next = manager->particles[particle_id].next;
    manager->particles[manager->particles[particle_id].next].last = manager->particles[particle_id].last;

    if (particle_id == manager->first_used) {
        manager->first_used = manager->particles[particle_id].next;
    }

    manager->particles[manager->first_free].last = particle_id;
    manager->particles[particle_id].next = manager->first_free;

    manager->particles[particle_id].last = -1;
    manager->first_free = particle_id;
    if (manager->particles[particle_id].life <= 0) {
        return -1;
    }
    return particle_id;
}

void CM_ManagerUpdate(CM_Manager *manager, double dt) {
    int index = manager->first_used;
    int next_index = 0;
    while (index >= 0) {
        if (manager->particles[index].dest_rect != NULL) {
            manager->particles[index].dest_rect->x += (float) (manager->particles[index].x_vel * dt);
            manager->particles[index].dest_rect->x += (float) (manager->particles[index].y_vel * dt);
            manager->particles[index].dest_rect->w += (float) (manager->particles[index].width_vel * dt);
            manager->particles[index].dest_rect->h += (float) (manager->particles[index].height_vel * dt);
        }
        manager->particles[index].life -= dt;

        next_index = manager->particles[index].next;
        if (manager->particles[index].life <= 0) {
            CM_ManagerKillParticle(manager, index);
        }
        index = next_index;
    }
}

void CM_ManagerDraw(CM_Manager *manager, SDL_Renderer *renderer) {
    int index = manager->first_used;
    while (index >= 0) {
        SDL_RenderCopy(
                renderer,
                manager->particles[index].texture,
                manager->particles[index].source_rect,
                manager->particles[index].dest_rect
                );
        index = manager->particles[index].next;
    }
}

void CM_DestroyManager(CM_Manager *manager) {
    free(manager->particles);
    free(manager);
}

