#ifdef _WIN32
#include <windows.h>
#include "game/DemoGame.hpp"

// The WinMain function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Initialize any necessary Windows-specific structures or settings here
	// For instance, you can parse lpCmdLine for command line arguments

	// Create and run the game
	DemoGame game;
	game.Run();

	// Return success
	return 0;
}
#else
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "game/DemoGame.hpp"

int main(int ArgCount, char** Args)
{
	DemoGame game;
	game.Run();
	return 0;
}
#endif
