#include <SDL2pp/SDL2pp.hh>
#include "Message_generated.h"

#include "GameDefs.h"
#include "Game.h"
#include "Network.h"
#include "Timer.h"
#include "Debug.h"
#include "Ignore.h"

#include <exception>
#include <memory>
#include <atomic>
#include <thread>

using namespace AM;
using namespace AM::Server;

int inputThread(std::atomic<bool>* exitRequested)
{
    while (!(*exitRequested)) {
        std::string userInput = "";
        std::getline(std::cin, userInput);
        if (userInput == "exit") {
            *exitRequested = true;
        }
    }

    return 0;
}

int main(int argc, char **argv)
try
{
    // SDL2 needs this signature for main, but we don't use the parameters.
    ignore(argc);
    ignore(argv);

    // Set up the network utility.
    Network network;

    // Set up our game.
    Game game(network);

    // Spin up a thread to check for command line input.
    std::atomic<bool> exitRequested = false;
    std::thread inputThreadObj(inputThread, &exitRequested);

    DebugInfo("Starting main loop.");

    Timer timer;
//    Timer exitTimer;
    // Prime the timer so it doesn't start at 0.
//    exitTimer.updateSavedTime();
    timer.updateSavedTime();
    while (!exitRequested) {
        // Calc the time delta.
        double deltaSeconds = timer.getDeltaSeconds(true);

        // Run the game.
        game.tick(deltaSeconds);

        // Send waiting messages.
        network.sendWaitingMessages(deltaSeconds);

        // Check if we overran.
        double executionSeconds = timer.getDeltaSeconds(false);
        if (executionSeconds >= GAME_TICK_INTERVAL_S) {
            DebugInfo("Overran the game tick rate.");
        }

//        if (exitTimer.getDeltaSeconds(false) > 600) {
//            break;
//        }
    }

//    DebugInfo("Done.");

    inputThreadObj.join();

    return 0;
}
catch (SDL2pp::Exception& e) {
    std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
    std::cerr << "  Reason:  " << e.GetSDLError() << std::endl;
    return 1;
}
catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
