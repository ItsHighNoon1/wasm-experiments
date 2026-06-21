#include <stdio.h>
#include <emscripten.h>
#include <SDL3/SDL.h>

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} Texture_t;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static Texture_t image = {};
static float image_x;
static float image_y;
static float image_vx;
static float image_vy;

#define IMAGE_SPEED 0.01f
#define IMAGE_WIDTH 0.1f
#define IMAGE_HEIGHT 0.1f

void loop() {
    int window_width;
    int window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    SDL_SetRenderDrawColor(renderer, 98, 211, 162, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    image_x += image_vx;
    image_y += image_vy;
    if (image_x < 0.0f) {
        image_x = 0.0f;
        image_vx = IMAGE_SPEED;
    } else if (image_x + IMAGE_WIDTH >= 1.0f) {
        image_x = 1.0f - IMAGE_WIDTH;
        image_vx = -IMAGE_SPEED;
    }
    if (image_y < 0.0f) {
        image_y = 0.0f;
        image_vy = IMAGE_SPEED;
    } else if (image_y + IMAGE_HEIGHT >= 1.0f) {
        image_y = 1.0f - IMAGE_HEIGHT;
        image_vy = -IMAGE_SPEED;
    }
    SDL_FRect dst_rect;
    dst_rect.x = image_x;
    dst_rect.y = image_y;
    dst_rect.w = IMAGE_WIDTH;
    dst_rect.h = IMAGE_HEIGHT;
    
    SDL_RenderTexture(renderer, image.texture, NULL, &dst_rect);
    SDL_RenderPresent(renderer);
}

void image_load_success(unsigned int handle, void* user_data, const char* filename) {
    printf("Got %s from network\n", filename);
    Texture_t* dst = (Texture_t*)user_data;
    SDL_Surface* surface = SDL_LoadPNG(filename);
    if (!surface) {
        printf("Failed to load %s from virtual FS: %s\n", filename, SDL_GetError());
        exit(1);
    }
    dst->texture = SDL_CreateTextureFromSurface(renderer, surface);
    dst->width = surface->w;
    dst->height = surface->h;
    if (!dst->texture) {
        printf("Failed to create texture for %s: %s\n", filename, SDL_GetError());
        exit(1);
    }
    SDL_DestroySurface(surface);
}

void image_load_failure(unsigned int handle, void* user_data, int status) {
    printf("Failed to load image with status %d\n", status);
    exit(1);
}

int main() {
    emscripten_async_wget2("https://cdn2.cdnstep.com/hkiKBJyFdECnzhaKBvy4/cover-1.thumb256.png", "res/test.png", 
        "GET", NULL, &image, image_load_success, image_load_failure, NULL);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Failed SDL_Init\n");
        exit(1);
    }
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_CANVAS_SELECTOR, "#canvas");

    // Window title goes to <title>
    if (!SDL_CreateWindowAndRenderer(NULL, 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        printf("Failed SDL_CreateWindowAndRenderer\n");
        exit(1);
    }
    SDL_SetRenderLogicalPresentation(renderer, 1, 1, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    int fail_count = 0;
    while (!image.texture) {
        printf("Image hasn't loaded yet...\n");
        if (fail_count++ > 10) {
            printf("Timed out\n");
            exit(1);
        }
        emscripten_sleep(500);
    }

    image_x = 0.5f;
    image_y = 0.2f;
    image_vx = 0.01f;
    image_vy = 0.01f;
    emscripten_set_main_loop(loop, 0, true);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}