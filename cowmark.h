#ifndef COWMARK_HEADER_GUARD
#define COWMARK_HEADER_GUARD
#include <SDL2/SDL.h>

typedef struct {
    // Must have the same Renderer as its manager
    SDL_Texture *texture;
    // Dest rect
    SDL_FRect *dest_rect;
    // Source rect
    SDL_Rect *source_rect;
    // Rates of change, in pixels per second
    double width_vel, height_vel, x_vel, y_vel;
    // Rates of change, in degrees per second
    double rotation, rotation_speed;
    // Alive time left, in seconds
    double life;
    // Particles are in a doubly linked list for easy insertion/deletion
    // These are indexes in the manager
    int next;
    int last;
} CM_Particle;

typedef struct {
    CM_Particle *particles;
    SDL_FRect *dest_rects;
    SDL_Rect *source_rects;
    int max_particles;
    int particle_count;
    int first_free;
    int first_used;
} CM_Manager;

CM_Manager *CM_CreateManager(int max_particles);

// TODO: Reduce param count here?
int CM_ManagerAddParticle(CM_Manager *manager, SDL_Texture *texture, SDL_FRect *dest_rect, SDL_Rect *source_rect, double width_vel, double height_vel, double x_vel, double y_vel, double life);

int CM_ManagerKillParticle(CM_Manager *manager, int particle_id);

int CM_ManagerGetParticleCount(CM_Manager *manager);

void CM_ManagerUpdate(CM_Manager *manager, double dt);

void CM_ManagerDraw(CM_Manager *manager, SDL_Renderer *renderer);

void CM_DestroyManager(CM_Manager *manager);

#endif