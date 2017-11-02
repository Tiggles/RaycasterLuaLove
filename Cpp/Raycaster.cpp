#include <SDL2/SDL.h>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <unistd.h> // usleep


int worldMap[24][24];/* =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};*/

#define SCREEN_HEIGHT 1080 
#define SCREEN_WIDTH 1920

int main() {
  double posX = 21.0f, posY = 11.0f;
  double dirX = -1.0f, dirY = 0.0f;
  double planeX = 0.0f, planeY = 0.66f;
  long dt, lastTime, now;
  bool running = true;
  
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

  lastTime = time(0);
  
  int max = 0;

  while (running) {
    long now = time(0);
    dt = now - lastTime;
    lastTime = now;
    double moveSpeed = 0.5;
    double rotSpeed = 0.01;
    printf("%lu, %lu, %lu\n", now, lastTime, dt);


    SDL_Event event;
    bool up = false, down = false, left = false, right = false, esc = false;
    while ( SDL_PollEvent(&event) ) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                up = true;
                break;
            case SDLK_DOWN:
                down = true;
                break;
            case SDLK_LEFT:
                left = true;
                break;
            case SDLK_RIGHT:
                right = true;
                break;
            case SDLK_ESCAPE:
                esc = true;
                running = false;
                break;
            default:
                break;
        }
    }
    if (up) {
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0)
            posX = posX + dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0)
            posY = posY + dirY * moveSpeed;
    }
  
    if (down) {
        if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0)
            posX = posX - dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0)
            posY = posY - dirY * moveSpeed;
    }

    double oldDirX, oldPlaneX;

    if (right) {
        oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }

    if (left) {
        oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }


    SDL_SetRenderDrawColor(renderer, 102, 123, 123, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, topRect);
    SDL_SetRenderDrawColor(renderer, 156, 123, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, bottomRect);
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      double cameraX = 2 * double(x) / SCREEN_WIDTH - 1;
      double rayPosX = posX, rayPosY = posY;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      double mapX = int(rayPosX);
      double mapY = int(rayPosY);

      double deltaDistX = sqrt(1.0f + (rayDirY * rayDirY) / (rayDirX * rayDirX));
      double deltaDistY = sqrt(1.0f + (rayDirX * rayDirX) / (rayDirY * rayDirY));

      bool hit = false;
      int stepX, stepY; // Other type?
      double sideDistX, sideDistY; 

      if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (rayPosX - mapX) * deltaDistX;
      } else {
        stepX = 1;
        sideDistX = (mapX + 1.0f - rayPosX) * deltaDistX;
      }
      if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (rayPosY - mapY) * deltaDistY;
      } else {
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
        } else {
          sideDistY = sideDistY + deltaDistY;
          mapY = mapY + stepY;
          side = 1;
        }
        if (worldMap[int(mapX)][int(mapY)] > 0)
            hit = true;
      }
      
      if (side == 0) {
        perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
      } else {
        perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;
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
      } else if (1 == mapValue) { 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  
      } else if (2 == mapValue) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
      } else if (3 == mapValue) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      }
      SDL_RenderDrawLine(renderer, x, drawEnd, x, drawStart);
      max = std::max(max, x);
    }
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);    
  }
  printf("%i\n", max);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
