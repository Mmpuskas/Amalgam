#include "NetworkUpdateSystem.h"
#include "Game.h"
#include "World.h"
#include "Network.h"
#include "MessageTools.h"
#include "EntityUpdate.h"
#include "Debug.h"

namespace AM
{
namespace Server
{

NetworkUpdateSystem::NetworkUpdateSystem(Game& inGame, World& inWorld, Network& inNetwork)
: game(inGame)
, world(inWorld)
, network(inNetwork)
{
}

void NetworkUpdateSystem::sendClientUpdates()
{
    // Collect the dirty entities so we don't need to re-find them for every client.
    std::vector<EntityID> dirtyEntities;
    dirtyEntities.reserve(MAX_ENTITIES);
    for (EntityID i = 0; i < MAX_ENTITIES; ++i) {
        if (world.entityIsDirty[i]) {
            dirtyEntities.push_back(i);
        }
    }

    /* Update clients as necessary. */
    for (EntityID entityID = 0; entityID < MAX_ENTITIES; ++entityID) {
        if ((world.componentFlags[entityID] & ComponentFlag::Client)) {
            // Send the entity whatever data it needs.
            constructAndSendUpdate(entityID, dirtyEntities);
        }
    }

    // Clean any dirty entities.
    for (EntityID dirtyEntity : dirtyEntities) {
        world.entityIsDirty[dirtyEntity] = false;
    }
}

void NetworkUpdateSystem::constructAndSendUpdate(EntityID entityID,
                                                 std::vector<EntityID>& dirtyEntities)
{
    /** Fill the vector of entities to send. */
    EntityUpdate entityUpdate{};
    ClientComponent& clientComponent = world.clients.find(entityID)->second;
    if (!clientComponent.isInitialized) {
        // New client, we need to send it all relevant entities.
        for (EntityID i = 0; i < MAX_ENTITIES; ++i) {
            if ((i != entityID)
                  && (world.componentFlags[i] & ComponentFlag::Client)) {
                serializeEntity(i, entityUpdate.entities);
            }
        }
        clientComponent.isInitialized = true;
    }
    else {
        // We only need to update the client with dirty entities.
        for (EntityID dirtyEntID : dirtyEntities) {
            serializeEntity(dirtyEntID, entityUpdate.entities);
        }
    }

    /* If there are updates to send, send an update message. */
    if (entityUpdate.entities.size() > 0) {
        DebugInfo("Queueing message for entity: %u with tick: %u", entityID,
                  game.getCurrentTick());

        // Finish filling the EntityUpdate.
        entityUpdate.tickNum = game.getCurrentTick();

        // Serialize the EntityUpdate.
        BinaryBufferSharedPtr messageBuffer = std::make_shared<BinaryBuffer>(
            Peer::MAX_MESSAGE_SIZE);
        std::size_t messageSize = MessageTools::serialize(*messageBuffer, entityUpdate,
            MESSAGE_HEADER_SIZE);

        // Fill the buffer with the appropriate message header.
        MessageTools::fillMessageHeader(MessageType::ClientInputs, messageSize,
            messageBuffer, 0);

        // Send the message.
        network.send(clientComponent.networkID, messageBuffer, entityUpdate.tickNum);
    }
}

void NetworkUpdateSystem::serializeEntity(EntityID entityID, std::vector<Entity>& entities)
{
    /* Fill the message with the latest PositionComponent, MovementComponent,
       and InputComponent data. */
    InputComponent& input = world.inputs[entityID];
    PositionComponent& position = world.positions[entityID];
    MovementComponent& movement = world.movements[entityID];

    Uint32 flags = (ComponentFlag::Position & ComponentFlag::Movement
                    & ComponentFlag::Input);

    // TEMP - Doing this until C++20 where we can emplace brace initializers.
    entities.push_back({entityID, flags, input, position, movement});

//    DebugInfo("Sending: (%f, %f), (%f, %f)", position.x, position.y, movement.velX,
//        movement.velY);
}

} // namespace Server
} // namespace AM
