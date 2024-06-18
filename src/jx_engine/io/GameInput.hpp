#ifndef GAMEINPUT_HPP
#define GAMEINPUT_HPP

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <unordered_map>
#include <iostream>
#include <vector>

class GameInput {
    public:

        enum Control {
        
        VehicleAccelerate = 0,
        VehicleBrake,
        VehicleDown,
        VehicleUp,
        VehicleTurnLeft,
        VehicleTurnRight,

        Idle, //No Input

        _MaxControls

        };
        
        typedef std::vector<Control> ActiveInput;
        
        void keyboardUpdateInput(const Uint8 *state);
        void controllerUpdateInput();
        void getActiveControls();

        Control currentAcceleration = Idle;
        Control currentTurn = Idle;

    private:
        ActiveInput activeControlsBuffer;

};

#endif