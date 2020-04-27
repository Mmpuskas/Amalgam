#include "Game.h"
#include "Network.h"
#include <iostream>
#include <iomanip>

extern bool exitRequested;

namespace AM
{
namespace Client
{

Game::Game(Network& inNetwork, std::shared_ptr<SDL2pp::Texture>& inSprites)
: world()
, network(inNetwork)
, playerInputSystem(world, network)
, networkMovementSystem(world, network)
, movementSystem(world)
, builder(BUILDER_BUFFER_SIZE)
, timeSinceTick(0.0f)
, sprites(inSprites)
{
}

void Game::connect()
{
    while (!(network.connect())) {
        std::cerr << "Network failed to connect. Retrying." << std::endl;
    }

    // Wait for the player's ID from the server.
    BinaryBufferPtr responseBuffer = network.receive();
    while (responseBuffer == nullptr) {
        responseBuffer = network.receive();
        SDL_Delay(10);
    }

    // Get the player ID from the connection response.
    const fb::Message* message = fb::GetMessage(responseBuffer->data());
    if (message->content_type() != fb::MessageContent::ConnectionResponse) {
        std::cerr << "Expected ConnectionResponse but got something else." << std::endl;
    }
    auto connectionResponse = static_cast<const fb::ConnectionResponse*>(message->content());
    EntityID player = connectionResponse->entityID();

    // Set up our systems.
    PlayerInputSystem playerInputSystem(world, network);
    NetworkMovementSystem networkMovementSystem(world, network);
    MovementSystem movementSystem(world);

    // Set up our player.
    SDL2pp::Rect textureRect(0, 32, 16, 16);
    SDL2pp::Rect worldRect(connectionResponse->x(), connectionResponse->y(), 64, 64);

    world.AddEntity("Player", player);
    world.positions[player].x = connectionResponse->x();
    world.positions[player].y = connectionResponse->y();
    world.movements[player].maxVelX = 250;
    world.movements[player].maxVelY = 250;
    world.sprites[player].texturePtr = sprites;
    world.sprites[player].posInTexture = textureRect;
    world.sprites[player].posInWorld = worldRect;
    world.AttachComponent(player, ComponentFlag::Input);
    world.AttachComponent(player, ComponentFlag::Movement);
    world.AttachComponent(player, ComponentFlag::Position);
    world.AttachComponent(player, ComponentFlag::Sprite);
    world.registerPlayerID(player);
}

void Game::tick(float deltaSeconds)
{
    timeSinceTick += deltaSeconds;
    if (timeSinceTick < GAME_TICK_INTERVAL_S) {
        // It's not yet time to process the game tick.
        return;
    }
    if ((timeSinceTick - GAME_TICK_INTERVAL_S) > .001) {
        std::cout << "Game overrun: " << (timeSinceTick - GAME_TICK_INTERVAL_S)
        << std::endl;
    }

    // Will return Input::Type::Exit if the app needs to exit.
    Uint32 start = SDL_GetTicks();
    Input input = playerInputSystem.processInputEvents();
    if (input.type == Input::Exit) {
        exitRequested = true;
    }
    Uint32 result = SDL_GetTicks() - start;
    if (result > 1) {
        std::cout << "Input time: " << result << std::endl;
    }

    // Run all systems.
    networkMovementSystem.processServerMovements();

    movementSystem.processMovements(timeSinceTick);

    timeSinceTick = 0;
}

World& Game::getWorld()
{
    return world;
}

} // namespace Client
} // namespace AM
