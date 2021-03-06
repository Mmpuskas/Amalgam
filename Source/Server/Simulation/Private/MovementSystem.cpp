#include "MovementSystem.h"
#include "MovementHelpers.h"
#include "World.h"
#include "Input.h"
#include "Position.h"
#include "Movement.h"
#include "SharedConfig.h"
#include "Log.h"
#include "Ignore.h"
#include "Profiler.h"

namespace AM
{
namespace Server
{
MovementSystem::MovementSystem(World& inWorld)
: world(inWorld)
{
    // Init the groups that we'll be using.
    auto group = world.registry.group<Input, Position, Movement>();
    ignore(group);
}

void MovementSystem::processMovements()
{
    SCOPED_CPU_SAMPLE(processMovements);

    /* Move all entities that have an input, position, and movement
       component. */
    auto group = world.registry.group<Input, Position, Movement>();
    for (entt::entity entity : group) {
        auto [input, position, movement]
            = group.get<Input, Position, Movement>(entity);

        // Process their movement.
        MovementHelpers::moveEntity(position, movement, input.inputStates,
                                    SharedConfig::SIM_TICK_TIMESTEP_S);
    }
}

} // namespace Server
} // namespace AM
