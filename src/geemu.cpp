#include "geemu.h"
#include "TextureManager.h"
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
int m_currentFrame;
Geemu::Geemu(){
 m_pWindow=NULL;
 m_pRenderer=NULL;
 m_bRunning=false;
}
 SDL_Texture* Geemu::str_to_texture(std::string str){
 SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, str.c_str(), {0,255,0} );
 SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_pRenderer,textSurface);
 mWidth=textSurface->w;
 mHeight =textSurface->h;
 SDL_FreeSurface(textSurface);
 return textTexture;
}
bool Geemu::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
 Snake_block.push_back({block_size, block_size});
 if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
 {
 std::cout << "SDL init success\n";
 m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
 if(m_pWindow != 0)
 {
 std::cout << "window creation success\n";
 m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_PRESENTVSYNC);
 if(m_pRenderer != 0)
 {
 std::cout << "renderer creation success\n";
 SDL_SetRenderDrawColor(m_pRenderer,0,0,0,0);}
 else
 {
 std::cout << "renderer init fail\n";
 return false;
 }
 }
 else
 {
 std::cout << "window init fail\n";
 return false; // window init fail
 }
 }
 else
 {
 std::cout << "SDL init fail\n";
 return false; // SDL init fail
 }
 std::cout << "init success\n";
 m_bRunning = true;
 gameOver = true;
 Delay=200;
 TheTextureManager::Instance()->load("assets/tail_up.png", "tail_up", m_pRenderer);
 TheTextureManager::Instance()->load("assets/body_vertical.png", "body_vertical", m_pRenderer);
 TheTextureManager::Instance()->load("assets/head_up.png", "head_up", m_pRenderer);
 TheTextureManager::Instance()->load("assets/apple.png", "apple", m_pRenderer);
 TheTextureManager::Instance()->load("assets/brick.png", "brick", m_pRenderer);

 TTF_Init();
 gFont = TTF_OpenFont( "Raleway-Medium.ttf", 28 );
 apl.first=block_size*2;
 apl.second=block_size*2;
 return true;
}
void Geemu::render()
{
 SDL_RenderClear(m_pRenderer);
 for(int i = 0; i< 10; ++i){
     TheTextureManager::Instance()->draw("brick", i*block_size, 0, block_size, block_size , m_pRenderer);
     TheTextureManager::Instance()->draw("brick", i*block_size, 9*block_size, block_size , block_size , m_pRenderer);
     TheTextureManager::Instance()->draw("brick", 0, i*block_size, block_size, block_size , m_pRenderer);
     TheTextureManager::Instance()->draw("brick", 9*block_size, i*block_size, block_size , block_size , m_pRenderer);
 }

 TheTextureManager::Instance()->drawFrame("head_up", Snake_block[lengthofSnake-1].first,Snake_block[lengthofSnake-1].second, block_size , block_size , 1, 0, m_pRenderer, SDL_FLIP_NONE,angle);
 for(int i=1;i<lengthofSnake-1;++i)
 TheTextureManager::Instance()->draw("body_vertical", Snake_block[i].first,Snake_block[i].second, block_size , block_size , m_pRenderer);

 if(lengthofSnake > 1){
    if(Snake_block[1].first > Snake_block[0].first )
        TheTextureManager::Instance()->drawFrame("tail_up", Snake_block[0].first,Snake_block[0].second, block_size , block_size , 1, 0, m_pRenderer, SDL_FLIP_NONE, 270);
    else if(Snake_block[1].first < Snake_block[0].first )
        TheTextureManager::Instance()->drawFrame("tail_up", Snake_block[0].first,Snake_block[0].second, block_size , block_size , 1, 0, m_pRenderer, SDL_FLIP_NONE, 90);
    else if(Snake_block[1].second > Snake_block[0].second )
        TheTextureManager::Instance()->drawFrame("tail_up", Snake_block[0].first,Snake_block[0].second, block_size , block_size , 1, 0, m_pRenderer, SDL_FLIP_NONE, 0);
    else if(Snake_block[1].second < Snake_block[0].second )
        TheTextureManager::Instance()->drawFrame("tail_up", Snake_block[0].first,Snake_block[0].second, block_size , block_size , 1, 0, m_pRenderer, SDL_FLIP_NONE, 180);
 }

 TheTextureManager::Instance()->draw("apple", apl.first,apl.second, block_size , block_size , m_pRenderer);

 SDL_Rect k; k.x=610; k.y=690; k.w=mWidth; k.h=mHeight;
 SDL_RenderCopy(m_pRenderer,str_to_texture("Score: "+std::to_string(score)),NULL,&k);

 SDL_RenderPresent(m_pRenderer);
}
void Geemu::handleEvents()
{
 SDL_Event event;
    if(SDL_PollEvent(&event))
        {
    switch (event.type)
        {
        case SDL_QUIT:
        m_bRunning = false;
        break;
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym){
                case SDLK_LEFT:
                if(dirx!=block_size){
                dirx=-block_size;
                angle=180;}
                diry=0;
                break;
                case SDLK_RIGHT:
                if(dirx!=-block_size){
                dirx=block_size;
                angle=0;
                }
                diry=0;
                break;
                case SDLK_UP:
                dirx=0;
                if(diry!=block_size){
                diry=-block_size;
                angle=270;
                }
                break;
                case SDLK_DOWN:
                dirx=0;
                if(diry!=-block_size){
                diry=block_size;
                angle=90;
                }
                break;
                case SDLK_p:
                gameOver=true;
                break;
                }
            }
        default:
        break;
            }
        }
}
void Geemu::clean()
{
 std::cout << "cleaning Geemu\n";
 TTF_CloseFont( gFont );
 gFont = NULL;
 SDL_DestroyWindow(m_pWindow);
 SDL_DestroyRenderer(m_pRenderer);
 SDL_Quit();
}
void Geemu::update()
{
 if(SDL_GetTicks()>Mark+Delay){
 int a=Snake_block[0].first,b=Snake_block[0].second;
 if(Snake_block[lengthofSnake-1].first+dirx>block_size*9){
    m_bRunning=false;
 }
 if(Snake_block[lengthofSnake-1].first+dirx<0){
    m_bRunning=false;
 }
 if(Snake_block[lengthofSnake-1].second+diry>block_size*9){
    m_bRunning=false;
 }
 if(Snake_block[lengthofSnake-1].second+diry<0){
    m_bRunning=false;
 }
 if(Snake_block[lengthofSnake-1].first+dirx!=apl.first||Snake_block[lengthofSnake-1].second+diry!=apl.second){
    for(int i=0;i<lengthofSnake-1;++i) {if(Snake_block[i]==Snake_block[lengthofSnake-1]){m_bRunning=false; break;}  Snake_block[i]=Snake_block[i+1];}
    Snake_block[lengthofSnake-1].first+=dirx;
    Snake_block[lengthofSnake-1].second+=diry;
 }
 else{
    Snake_block.push_back({Snake_block[lengthofSnake-1].first+dirx,Snake_block[lengthofSnake-1].second+diry});
    ++lengthofSnake;
    ++score;
    apl.first=(rand()%8 + 1)*block_size;
    apl.second=(rand()%8 + 1)*block_size;
    int check=true;
    while(check){
    check=false;
    for(int i=0;i<lengthofSnake;++i){
        if(apl.first==Snake_block[i].first&&apl.second==Snake_block[i].second){
        apl.first=(rand()%8 + 1)*block_size;
        apl.second=(rand()%8 + 1)*block_size;
        check=true;
        break;
        }
    }
    }
 }
 Mark=SDL_GetTicks();
 }
}
