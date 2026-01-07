#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <hip/hip_runtime.h>

#define TRUE 1
#define FALSE 0
#define PI 3.14159265359

#define rozdzielczosc 360.0f
#define windowRealResolution 800

#define polRozdzielczosc  rozdzielczosc/2


#define rozmiarMapy 100

unsigned char turnON = 1;
unsigned char EscON = 0;

typedef struct {double x; double y; double z; } obiektStatyczny;
typedef struct {obiektStatyczny xyz; double azymutalny; double zenitalny; } obiektDynamiczny;


obiektDynamiczny gracz = {0,0,0, 4.77,2.3};


obiektStatyczny * mapaTablica;
unsigned int mapaTablicaLicznik = 2;




unsigned char szukanie(int x, int y, int z  ) {
    for (int i = 1; i < mapaTablicaLicznik; i++) {
        if (mapaTablica[i].x == x && mapaTablica[i].y == y && mapaTablica[i].z == z  ) return TRUE;
    }
    return FALSE;
}
int main(void) {
    mapaTablica = malloc(sizeof(obiektStatyczny) * mapaTablicaLicznik);
    obiektStatyczny meow = { 2,2,0};
    mapaTablica[mapaTablicaLicznik -1] = meow;
    SDL_Window* meowOkno = SDL_CreateWindow ("FajneOkno",windowRealResolution,windowRealResolution,windowRealResolution,windowRealResolution,SDL_WINDOW_SHOWN);
    SDL_Renderer* meowRender = SDL_CreateRenderer (meowOkno, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event meowEvent;
    SDL_RenderSetLogicalSize(meowRender,rozdzielczosc,rozdzielczosc);

    while (turnON) {

        obiektDynamiczny pG = gracz;
        for (int yKrokow = polRozdzielczosc * -1; yKrokow <= polRozdzielczosc; yKrokow++ ) {
            for (int xKrokow = -polRozdzielczosc; xKrokow <= polRozdzielczosc; xKrokow++) {

                double FOVX = PI /3.0f;
                double FOVY = PI / 6.0f;

                double angleX = xKrokow/rozdzielczosc * FOVX;
                double angleY = yKrokow/rozdzielczosc * FOVY;

                double zen = pG.zenitalny+angleY;
                double azi = pG.azymutalny+angleX;

                double stepX = cos(zen) * sin(azi);
                double stepY = sin(zen);
                double stepZ = cos(zen) * cos(azi);

                obiektDynamiczny pS = pG;
                unsigned char colorIntensiv = 255;

                while (1) {
                    pS.xyz.x += stepX; pS.xyz.y += stepY; pS.xyz.z += stepZ;

                    if (szukanie((int)pS.xyz.x, (int)pS.xyz.y, (int)pS.xyz.z) ) break;

                    if (colorIntensiv >= 6) colorIntensiv-= 6;

                    if (colorIntensiv < 1 || pS.xyz.x > rozmiarMapy ||  pS.xyz.y > rozmiarMapy ||  pS.xyz.z > rozmiarMapy
                    || pS.xyz.x < -rozmiarMapy ||  pS.xyz.y < -rozmiarMapy ||  pS.xyz.z < -rozmiarMapy ) {colorIntensiv = 0; break; }
                }

                SDL_SetRenderDrawColor(meowRender, colorIntensiv,colorIntensiv,colorIntensiv,0);
                SDL_RenderDrawPoint(meowRender,xKrokow + polRozdzielczosc,yKrokow + polRozdzielczosc );
            }
        }

        //secition of SDL2
        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) { turnON = 0; }
            else if (meowEvent.type == SDL_KEYDOWN) {

                if (meowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    EscON++;
                    if (EscON > 1) {EscON = 0;}
                    SDL_ShowCursor(SDL_ENABLE);
                }
                else if (meowEvent.key.keysym.sym == SDLK_SPACE) gracz.xyz.y -= 0.50;
                else if (meowEvent.key.keysym.sym == SDLK_LSHIFT ) gracz.xyz.y += 0.50;
                else if (meowEvent.key.keysym.sym == SDLK_w || meowEvent.key.keysym.sym == SDLK_s) {
                    int mnoznik = 1;
                    if (meowEvent.key.keysym.sym == SDLK_s) {mnoznik = -1;}
                    gracz.xyz.x += sin(gracz.azymutalny) * (double)mnoznik;
                    gracz.xyz.z += cos(gracz.azymutalny) * (double)mnoznik;
                }
                else if (meowEvent.key.keysym.sym == SDLK_a || meowEvent.key.keysym.sym == SDLK_d) {
                    int mnoznik = -1;
                    if (meowEvent.key.keysym.sym == SDLK_a) {mnoznik = 1;}
                    gracz.xyz.x +=  cos(gracz.azymutalny) * (double)mnoznik;
                    gracz.xyz.z +=  sin(gracz.azymutalny) * (double)mnoznik;
                }

                else if (meowEvent.key.keysym.sym == SDLK_LEFT ||  meowEvent.key.keysym.sym == SDLK_RIGHT || meowEvent.key.keysym.sym == SDLK_UP ||   meowEvent.key.keysym.sym == SDLK_DOWN) {
                    if (gracz.azymutalny > 2*PI ) {gracz.azymutalny = 0.02;}
                    else if (gracz.azymutalny < 0 ) { gracz.azymutalny = 2*PI - 0.02;}
                    if (gracz.zenitalny > 2*PI ) {gracz.zenitalny = 0.02;}
                    else if (gracz.zenitalny < 0 ) { gracz.zenitalny = 2*PI - 0.02;}
                }
            }
            else if (!FALSE && (meowEvent.type == SDL_MOUSEMOTION && !EscON && (meowEvent.motion.yrel || meowEvent.motion.xrel) )  )
            {
                SDL_ShowCursor(SDL_DISABLE);
                gracz.azymutalny += meowEvent.motion.xrel /100.0f;
                if (!((int)gracz.zenitalny < -2 && meowEvent.motion.yrel == -1)&&!((int)gracz.zenitalny > 2 && meowEvent.motion.yrel == 1)) gracz.zenitalny += meowEvent.motion.yrel /100.0f;
                if (gracz.azymutalny > 2*PI ) {gracz.azymutalny = 0.02;}
                else if (gracz.azymutalny < 0 ) { gracz.azymutalny = 2*PI - 0.02;}
                SDL_WarpMouseInWindow(meowOkno, windowRealResolution / 2, windowRealResolution / 2);
            }

        }

        printf ("%d|%d|%d|%f|%f\n",(int)gracz.xyz.x,(int)gracz.xyz.y,(int)gracz.xyz.z,gracz.azymutalny,gracz.zenitalny);

        SDL_RenderPresent(meowRender);
        SDL_RenderClear(meowRender);
    }

}