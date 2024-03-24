/*
 Copyright © 2023 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <iostream>


#include <SDL2/SDL.h>
#include <stdio.h>

#include "graphics.hpp"

static bool isRunning = true;
static SDL_Renderer* renderer;
static SDL_Texture* texture;
static SDL_Event event;


extern void setup(void);
extern void loop(void);

int arduino_thread(void* data) {
    loop();
    isRunning = false;
    return 0;
}

int main(int argc, const char * argv[])
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    //Create window
    SDL_Window* window = SDL_CreateWindow(
                                          "Arduino",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          320 * 2,
                                          240 * 2,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
                                          );
    if( NULL == window ) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
        (void) SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(
                                renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STATIC,
                                320,
                                240
                                );
    
    setup();
    SDL_Thread* thread = SDL_CreateThread( arduino_thread, "Arduino Thread",  NULL );
    
    while(isRunning) {
        SDL_UpdateTexture(texture, NULL, screen::vram(), 320 * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(15);
        SDL_PollEvent(&event);
        
        if (SDL_QUIT == event.type) break;
        
        switch ( event.type ) {
            case SDL_MOUSEBUTTONDOWN:
                
                break;
                
            case SDL_MOUSEBUTTONUP:
                
                break;
                
            case SDL_MOUSEMOTION:
                
                break;
                
            case SDL_KEYDOWN:
            {
                auto sys = event.key.keysym.sym;
            }
                break;
                
        }
    }

    SDL_DetachThread(thread);
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
