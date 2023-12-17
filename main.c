#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cowmark.h"
#include <stdlib.h>
#include <time.h>

#define MAX_PARTICLES 1000000
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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

    CM_Manager *manager = CM_CreateManager(MAX_PARTICLES);

    int quit = 0;
    Uint64 last_tick = 0;
    Uint64 this_tick = 0;
    double dt = 0;
    SDL_Event event;
    SDL_Rect dest_rect;
    dest_rect.w = 24;
    dest_rect.h = 16;
    while (!quit) {
        last_tick = this_tick;
        this_tick = SDL_GetTicks64();
        dt = (double) (this_tick - last_tick) / 1000.0;
        printf("%f\n", dt);
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit++;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y = 0;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                for (int i = 0; i < 100000; i++) {
                    dest_rect.x = (float) (mouse_x + (random() % 400) - 200);
                    dest_rect.y = (float) (mouse_y + (random() % 400) - 200);
                    CM_ManagerAddParticle(manager, tex, &dest_rect, NULL, 0, 0, 0, 0, 10);
                }
            }
        }
        CM_ManagerUpdate(manager, dt);
        SDL_RenderClear(renderer);
        CM_ManagerDraw(manager, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(delay_time(this_tick));
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(manager);
}
