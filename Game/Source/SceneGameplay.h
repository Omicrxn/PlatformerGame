#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "App.h"
#include "SceneManager.h"

#include "Map.h"
#include "Player.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "Heart.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"

#include "Timer.h"

#define MAX_CHECKPOINTS 1
#define MAX_HEARTS 1
#define MAX_COINS 1
#define MAX_FLYING_ENEMIES 1
#define MAX_WALKING_ENEMIES 1

class SceneGameplay : public Scene
{
public:

    SceneGameplay(App* app, SceneManager* sceneManager);
    virtual ~SceneGameplay();

    bool Load(Textures* tex, EntityManager* entityManager);

    bool Update(Input* input, Collisions*, float dt);

    bool Draw(Render* render);
    void DrawBackground(Render* render);
    bool Unload();
    void CollisionHandler();

private:

    App* app;
    SceneManager* sceneManager;

    Map* map;

    // Entities
    Player* player;
    EnemyFly* enemiesFly[MAX_FLYING_ENEMIES];
    EnemyWalk* enemiesWalk[MAX_WALKING_ENEMIES];
    Coin* coins[MAX_COINS];
    Heart* hearts[MAX_HEARTS];
    Checkpoint* checkpoints[MAX_CHECKPOINTS];

    SDL_Texture* playerTexture;
    SDL_Rect camera;
    iPoint tempPlayerPosition;

    // Background textures
    SDL_Texture* background1;
    SDL_Texture* background2;
    SDL_Texture* background3;
    SDL_Texture* background4;
    SDL_Rect backgroundRect;

    Font* font;

    Timer timer;

    EntityManager* entityManager;
};

#endif // __SCENEGAMEPLAY_H__