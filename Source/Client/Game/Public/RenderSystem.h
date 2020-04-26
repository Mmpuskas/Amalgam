#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

namespace AM
{
namespace Client
{

class World;

class RenderSystem
{
public:
    RenderSystem(World& inWorld);

    void collectRenderObjects();

    void render();

private:
    World& world;
};

} // namespace Client
} // namespace AM

#endif /* RENDERSYSTEM_H */