#include "cowmark.h"
#include <SDL2/SDL.h>

CM_Manager *CM_CreateManager(int max_particles) {
    CM_Manager *manager = malloc(sizeof(CM_Manager));
    manager->particles = malloc(sizeof(CM_Particle) * max_particles);
    manager->max_particles = max_particles;
    manager->particle_count = 0;
    manager->first_free = 0;
    // Every particle points to the next
    for (int i = 0; i < max_particles - 1; ++i) {
        manager->particles[i].next = i + 1;
    }
    // The last one is -1 for its next free, a list terminator value
    manager->particles[max_particles-1].next = -1;
    return manager;
};

// TODO: Reduce param count here?
int CM_ManagerAddParticle(CM_Manager *manager, SDL_Texture *texture, SDL_FRect dest_rect, SDL_FRect source_rect, double width_vel, double height_vel, double x_vel, double y_vel, double life) {
    int new_index = manager->first_free;
    // No free particles
    if (new_index < 0) {
        return -1;
    }
    CM_Particle new_particle = manager->particles[new_index];
    manager->first_free = new_particle.next;
    new_particle.data.texture = texture;
    new_particle.data.dest_rect = dest_rect;
    new_particle.data.source_rect = source_rect;
    new_particle.data.width_vel = width_vel;
    new_particle.data.height_vel = height_vel;
    new_particle.data.x_vel = x_vel;
    new_particle.data.y_vel = y_vel;
    new_particle.data.life = life;
};

int CM_ManagerKillParticle(CM_Manager *manager, int particle_id);

int CM_ManagerUpdate(CM_Manager *manager, double dt);

int CM_ManagerDraw(CM_Manager *manager, SDL_Renderer *renderer);

void CM_DestroyManager(CM_Manager *manager) {
    free(manager->particles);
    free(manager);
};

