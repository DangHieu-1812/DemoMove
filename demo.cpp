#include <iostream>
#include <SDL.h>
using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "DemoMove";
void logSDLError(std::ostream& os,const std::string &msg, bool fatal = false)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
struct Box {
    int x;
    int y;
    int size=40;
    void render(SDL_Renderer* renderer){
    SDL_Rect filled_rect;
    filled_rect.x = x;
    filled_rect.y = y;
    filled_rect.w = size;
    filled_rect.h = size;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // green
    SDL_RenderFillRect(renderer, &filled_rect);

    }
    void move(){
    x+=5;
    y+=5;
    }
    void moveleft()
    {
        x-=10;
    }
    void moveright()
    {
        x+=10;
    }
    void movedown()
    {
        y+=10;
    }
    void moveup()
    {
        y-=10;
    }
    bool inside(int minX,int minY, int maxX, int maxY){
        return (minX<=x && minY<=y && x+size<=maxX && y+size<=maxY);
    }
};
int main(int argc, char* argv[])
{
      SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    Box box;
    box.x=10;
    box.y=10;
    SDL_Event e;
    while(box.inside(0,0,SCREEN_WIDTH, SCREEN_HEIGHT)){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        box.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
        if ( SDL_PollEvent(&e) == 0) continue;
        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_KEYDOWN) {
        	switch (e.key.keysym.sym) {
        		case SDLK_ESCAPE:
        		    break;
        		case SDLK_LEFT: box.moveleft();
        			break;
            	case SDLK_RIGHT: box.moveright();
            		break;
            	case SDLK_DOWN: box.movedown();
					break;
            	case SDLK_UP: box.moveup();
            		break;
        		default: break;}}

    }
    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
