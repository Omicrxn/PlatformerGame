#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "App.h"
#include "SceneManager.h"

#include "Window.h"
#include "Audio.h"
#include "Map.h"
#include "Player.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "Heart.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

#define MAX_CHECKPOINTS 2
#define MAX_HEARTS 1
#define MAX_COINS 1
#define MAX_FLYING_ENEMIES 1
#define MAX_WALKING_ENEMIES 1

enum class MenuSelection;
enum class SettingsSelection;
class Coin;
class Heart;

class SceneGameplay : public Scene
{
public:

    SceneGameplay(App* app, SceneManager* sceneManager, Window* win, AudioManager* audio);
    virtual ~SceneGameplay();

    bool Load(Textures* tex, EntityManager* entityManager);

    bool Update(Input* input, Collisions*, float dt);

    bool Draw(Render* render);
    void DrawBackground(Render* render);
    bool Unload(Textures* tex, AudioManager* audio);
    void CollisionHandler();
    bool OnGuiMouseClickEvent(GuiControl* control);

    bool animateHeart;
    bool animateCoin;

private:

    App* app;
    SceneManager* sceneManager;
    AudioManager* audio;

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
    SDL_Rect menuRect;

    SDL_Rect mouseCursorRect[2];
    iPoint mousePos;
    bool clicking;
    int hoverFx;
    int clickFx;

    // Menu textures
    SDL_Texture* atlasGUITexture;
    SDL_Texture* barTexture;
    SDL_Rect barRect;
    SDL_Texture* heartTexture;
    SDL_Rect heartRect;
    SDL_Texture* coinTexture;
    SDL_Rect coinRect;

    Animation heartAnimation;
    
    Animation coinAnimation;

    Font* font;

    EntityManager* entityManager;
    Window* window;

    bool pause;

    // In-game menu buttons
    GuiButton* btnResume;
    GuiButton* btnSettings;
    GuiButton* btnTitle;
    GuiButton* btnExit;

    // Settings' sliders and checkboxes
    GuiSlider* sldrMusicVolume;
    GuiSlider* sldrFxVolume;
    GuiCheckBox* cbxFullscreen;
    GuiCheckBox* cbxVSync;

    MenuSelection menuCurrentSelection;
    SettingsSelection settingsCurrentSelection;

    bool fullscreen = false;
    bool vsync = false;

    bool debugCP = false;

    float timer;
};

#endif // __SCENEGAMEPLAY_H__