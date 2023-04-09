#include "SDL.h"
#include"geemu.h"
Geemu* g_game = 0;
int main(int argc, char* argv[])
{
 g_game = new Geemu();
 g_game->init("Hebi", 100, 100, 750, 750, 0);
 while(g_game->running())
 {
 g_game->handleEvents();
 g_game->update();
 g_game->render();

 }
 g_game->clean();
 return 0;
}
