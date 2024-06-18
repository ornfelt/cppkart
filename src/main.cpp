//#include <stdio.h>
//#include <stdint.h>
//#include <assert.h>
//#include "game/JXGame.hpp"
//
//int main(int ArgCount, char **Args)
//{
//  JXGame game;
//  game.Run();
//  return 0;
//}

#include <windows.h>
#include "game/JXGame.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    JXGame game;
    game.Run();
    return 0;
}