#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PARTICLES 100000
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    // Image
    SDL_Texture *tex;
    // Positioning
    SDL_FRect *rect;
    float width_vel;
    float height_vel;
    // Velocity (In Pixels per second)
    float vel_x;
    float vel_y;
    // Rotation (In degrees per second)
    float rotation;
    float rotation_speed;
    // Remaining life (In seconds)
    float life;
} Particle;

int add_particle(Particle *particle_list, float x, float y) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (particle_list[i].life <= 0) {
            particle_list[i].rect->x = x + (float) (random() % 200) - 100;
            particle_list[i].rect->y = y + (float) (random() % 200) - 100;
            particle_list[i].life = 5 + (float) (random() % 4);
            return 1;
        }
    }
    printf("MAX EXCEEDED!\n");
    return 0;
}

void process_particle(Particle *particle, float dt) {
    particle->rect->x += particle->vel_x * dt;
    particle->rect->x += particle->vel_y * dt;
    particle->rotation += particle->rotation_speed * dt;
    particle->rect->w += particle->width_vel * dt;
    particle->rect->h += particle->height_vel * dt;
    particle->life -= dt;
}

void handle_particles(Particle *particle_list, SDL_Renderer *renderer, float dt) {
    for (long long int i = 0; i < MAX_PARTICLES; ++i) {
        Particle *current = &(particle_list[i]);
        if (current->life <= 0) {
            continue;
        }
        process_particle(current, dt);
        SDL_RenderCopyExF(renderer, current->tex, NULL, current->rect, current->rotation, NULL, SDL_FLIP_NONE);
    }
}

Uint64 delay_time(Uint64 beginning_of_tick) {
    Uint64 now = SDL_GetTicks64();
    Uint64 next_time = beginning_of_tick + (1000 / 60);
    if (now >= next_time) {
        return 0;
    }
    return next_time - now;
}

int main() {

    srandom(time(NULL));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *tex = NULL;

    window = SDL_CreateWindow(
            "CCowmark",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    tex = SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/cow.png"));

    if (window == NULL || renderer == NULL || tex == NULL) {
        printf("Darn it SDL is broken!");
        return 1;
    }

    Particle *manager = malloc(MAX_PARTICLES * sizeof(Particle));
    for (long long int i = 0; i < MAX_PARTICLES; ++i) {
        manager[i] = (Particle) {
            tex,
            malloc(sizeof(SDL_FRect)),
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f
        };
        manager[i].rect->x = 0;
        manager[i].rect->y = 0;
        manager[i].rect->w = 24;
        manager[i].rect->h = 16;
    }

    int quit = 0;
    Uint64 last_tick = 0;
    Uint64 this_tick = 0;
    float dt = 0;
    SDL_Event event;
    while (!quit) {
        last_tick = this_tick;
        this_tick = SDL_GetTicks64();
        dt = (this_tick - last_tick) / 1000.0;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit++;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y = 0;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                for (int i = 0; i < 100; i++) {
                    add_particle(manager, (float) mouse_x, (float) mouse_y);
                }
            }
        }
        SDL_RenderClear(renderer);
        handle_particles(manager, renderer, dt);
        SDL_RenderPresent(renderer);
        SDL_Delay(delay_time(this_tick));
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(manager);
}
