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

    bool animateHeart = false;
    bool animateCoin = false;

private:

    App* app = nullptr;
    SceneManager* sceneManager = nullptr;
    AudioManager* audio = nullptr;

    Map* map = nullptr;

    // Entities
    Player* player = nullptr;
    EnemyFly* enemiesFly[MAX_FLYING_ENEMIES] = { nullptr };
    EnemyWalk* enemiesWalk[MAX_WALKING_ENEMIES] = { nullptr };
    Coin* coins[MAX_COINS] = { nullptr };
    Heart* hearts[MAX_HEARTS] = { nullptr };
    Checkpoint* checkpoints[MAX_CHECKPOINTS] = { nullptr };

    SDL_Texture* playerTexture = nullptr;
    SDL_Rect camera = { 0,0,0,0 };
    iPoint tempPlayerPosition = { 0,0 };

    // Background textures
    SDL_Texture* background1 = nullptr;
    SDL_Texture* background2 = nullptr;
    SDL_Texture* background3 = nullptr;
    SDL_Texture* background4 = nullptr;
    SDL_Rect backgroundRect = { 0,0,0,0 };
    SDL_Rect menuRect = { 0,0,0,0 };

    SDL_Rect mouseCursorRect[2] = { 0,0,0,0 };
    iPoint mousePos = { 0,0 };
    bool clicking = false;
    int hoverFx = -1;
    int clickFx = -1;

    // Menu textures
    SDL_Texture* atlasGUITexture = nullptr;
    SDL_Texture* barTexture = nullptr;
    SDL_Rect barRect = { 0,0,0,0 };
    SDL_Texture* heartTexture = nullptr;
    SDL_Rect heartRect = { 0,0,0,0 };
    SDL_Texture* coinTexture = nullptr;
    SDL_Rect coinRect = { 0,0,0,0 };

    Animation heartAnimation;
    
    Animation coinAnimation;

    Font* font = nullptr;

    EntityManager* entityManager = nullptr;
    Window* window = nullptr;

    bool pause = false;

    // In-game menu buttons
    GuiButton* btnResume = nullptr;
    GuiButton* btnSettings = nullptr;
    GuiButton* btnTitle = nullptr;
    GuiButton* btnExit = nullptr;

    // Settings' sliders and checkboxes
    GuiSlider* sldrMusicVolume = nullptr;
    GuiSlider* sldrFxVolume = nullptr;
    GuiCheckBox* cbxFullscreen = nullptr;
    GuiCheckBox* cbxVSync = nullptr;

    MenuSelection menuCurrentSelection;
    SettingsSelection settingsCurrentSelection;

    bool fullscreen = false;
    bool vsync = false;

    bool debugCP = false;

    bool checked = false;

    float timer = 0.0f;
};

#endif // __SCENEGAMEPLAY_H__