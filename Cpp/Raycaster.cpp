#include "SDL2/SDL.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>

struct Vector2D
{
    double x;
    double y;
};

struct Player
{
    Vector2D *position;
    Vector2D *direction;
    Vector2D *plane;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    double moveSpeed = 0.1;
    double rotSpeed = 0.02;
};

void Render(SDL_Renderer *renderer, Player *player, SDL_Rect *topRect, SDL_Rect *bottomRect);
bool HandleInput(Player *player, bool running, unsigned int deltaTime);
void Turn(Player *player, bool turnRight, unsigned int deltaTime);

int worldMap[24][24] =
{
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1 },
    { 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,1 },
    { 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

#define SCREEN_HEIGHT 1080 
#define SCREEN_WIDTH 1920



int main(int argc, char *argv[]) {
    // Init
    bool running = true;
    Player *player = new Player;
    player->direction = new Vector2D;
    player->direction->x = -1.0f;
    player->direction->y = 0.0f;
    player->position = new Vector2D;
    player->position->x = 21.0f;
    player->position->y = 11.0f;
    player->plane = new Vector2D;
    player->plane->x = 0.0f;
    player->plane->y = 0.66f;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_Rect *topRect = new SDL_Rect;
    SDL_Rect *bottomRect = new SDL_Rect;
    topRect->x = bottomRect->x = 0;
    topRect->y = 0;
    bottomRect->y = SCREEN_HEIGHT / 2;
    topRect->w = bottomRect->w = SCREEN_WIDTH;
    topRect->h = bottomRect->h = SCREEN_HEIGHT / 2;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    unsigned int last = SDL_GetTicks(), now, deltaTime;
    // Main Loop
    while (running) {
        now = SDL_GetTicks();
        deltaTime = now - last;
        last = now;

        running = HandleInput(player, running, deltaTime);
        Render(renderer, player, topRect, bottomRect);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

//void Turn(Player * player, double &oldDirX, double &oldPlaneX);

bool HandleInput(Player *player, bool running, unsigned int deltaTime)
{
    SDL_Event event;
    player->up = player->down = player->left = player->right = false;
    while (SDL_PollEvent(&event)) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            player->up = true;
            break;
        case SDLK_DOWN:
            player->down = true;
            break;
        case SDLK_LEFT:
            player->left = true;
            break;
        case SDLK_RIGHT:
            player->right = true;
            break;
        case SDLK_ESCAPE:
            running = false;
            break;
        case SDLK_PLUS:
            player->plane->y += 0.1f;
            break;
        case SDLK_MINUS:
            player->plane->y -= 0.1f;
            break;
        default:
            break;
        }
    }

    if (player->up) {
        if (worldMap[int(player->position->x + player->direction->x * player->moveSpeed)][int(player->position->y)] == 0)
            player->position->x = player->position->x + player->direction->x * player->moveSpeed * deltaTime;
        if (worldMap[int(player->position->x)][int(player->position->y + player->direction->y * player->moveSpeed)] == 0)
            player->position->y = player->position->y + player->direction->y * player->moveSpeed * deltaTime;
    }

    if (player->down) {
        if (worldMap[int(player->position->x - player->direction->x * player->moveSpeed)][int(player->position->y)] == 0)
            player->position->x = player->position->x - player->direction->x * player->moveSpeed * deltaTime;
        if (worldMap[int(player->position->x)][int(player->position->y - player->direction->y * player->moveSpeed)] == 0)
            player->position->y = player->position->y - player->direction->y * player->moveSpeed * deltaTime;
    }

    if (player->right) {
        Turn(player, true, deltaTime);
    }
    if (player->left)
        Turn(player, false, deltaTime);
    return running;
}

void Turn(Player *player, bool turnRight, unsigned int deltaTime)
{
    double rotation = 0;
    rotation = turnRight ? -player->rotSpeed  * deltaTime : player->rotSpeed  * deltaTime;
    double oldDirX, oldPlaneX;
    oldDirX = player->direction->x;
    player->direction->x = player->direction->x * cos(rotation) - player->direction->y * sin(rotation);
    player->direction->y = oldDirX * sin(rotation) + player->direction->y * cos(rotation);
    oldPlaneX = player->plane->x;
    player->plane->x = player->plane->x * cos(rotation) - player->plane->y * sin(rotation);
    player->plane->y = oldPlaneX * sin(rotation) + player->plane->y * cos(rotation);
}

void Render(SDL_Renderer *renderer, Player *player, SDL_Rect *topRect, SDL_Rect *bottomRect) 
{
    SDL_SetRenderDrawColor(renderer, 102, 123, 123, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, topRect);
    SDL_SetRenderDrawColor(renderer, 156, 123, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, bottomRect);
    Vector2D *rayDirection = new Vector2D;
    for (int x = 0; x < SCREEN_WIDTH; x++) 
    {
        double cameraX = 2 * double(x) / SCREEN_WIDTH - 1;
        double rayPosX = player->position->x, rayPosY = player->position->y;
        rayDirection->x = player->direction->x + player->plane->x * cameraX;
        rayDirection->y = player->direction->y + player->plane->y * cameraX;

        double mapX = int(rayPosX);
        double mapY = int(rayPosY);

        double deltaDistX = sqrt(1.0f + (rayDirection->y * rayDirection->y) / (rayDirection->x * rayDirection->x));
        double deltaDistY = sqrt(1.0f + (rayDirection->x * rayDirection->x) / (rayDirection->y * rayDirection->y));

        bool hit = false;
        int stepX, stepY;
        double sideDistX, sideDistY;

        if (rayDirection->x < 0) {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0f - rayPosX) * deltaDistX;
        }
        if (rayDirection->y < 0) {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0f - rayPosY) * deltaDistY;
        }

        int side;
        double perpWallDist;

        while (hit == false) {
            if (sideDistX < sideDistY) {
                sideDistX = sideDistX + deltaDistX;
                mapX = mapX + stepX;
                side = 0;
            }
            else {
                sideDistY = sideDistY + deltaDistY;
                mapY = mapY + stepY;
                side = 1;
            }
            if (worldMap[int(mapX)][int(mapY)] > 0)
                hit = true;
        }

        if (side == 0) {
            perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirection->x;
        }
        else {
            perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirection->y;
        }

        int lineHeight = int(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) {
            drawStart = 0;
        }
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) {
            drawEnd = SCREEN_HEIGHT - 1;
        }

        int mapValue = worldMap[int(mapX)][int(mapY)];

        if (side == 1) {
            mapValue = int(mapValue / 2);
        }

        if (0 == mapValue) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
        }
        else if (1 == mapValue) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        }
        else if (2 == mapValue) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        }
        else if (3 == mapValue) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderDrawLine(renderer, x, drawEnd, x, drawStart);
    }
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}
