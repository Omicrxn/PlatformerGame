#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "Heart.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load(Textures* tex, EntityManager* entityManager);

    bool Update(Input* input, Collisions*, float dt);

    bool Draw(Render* render);
    void DrawBackground(Render* render);
    bool Unload();
    void CollisionHandler();
private:

    Map* map;

    Player* player;
    EnemyFly* enemyFly1;
    EnemyWalk* enemyWalk1;

    Coin* coin;
    Heart* heart;
    Checkpoint* checkpoint1;
    Checkpoint* checkpoint2;

    SDL_Texture* playerTexture;
    SDL_Rect camera;
    iPoint tempPlayerPosition;

    // Background textures
    SDL_Texture* background1;
    SDL_Texture* background2;
    SDL_Texture* background3;
    SDL_Texture* background4;
    SDL_Rect backgroundRect;
};

#endif // __SCENEGAMEPLAY_H__