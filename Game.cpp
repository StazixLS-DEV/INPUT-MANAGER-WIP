#include "Game.h"
#include "ActorManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "TimerManager.h"

using namespace Input;
using namespace Camera;

Game::Game()
{
	window = RenderWindow();
}


void Game::Start()
{
    window.create(VideoMode({ 800, 400 }), "Angry Birds");

    M_CAMERA.CreateCamera("DefaultCamera"); 
    //M_CAMERA.CreateCamera(Vector2f(), Vector2f(300.0f, 300.0f), "DefaultCamera");
    M_ACTOR.BeginPlay();

    ActionMap* _actionMap = M_INPUT.CreateActionMap("Demo");
    Action* _action = new Action("Test", ActionData(KeyboardPressed, CAST(int, Key::A)), [&]() { LOG(Display, "coucou"); });
    Action* _action2 = new Action("Test2", ActionData(KeyboarReleased, CAST(int, Key::A)), [&]() { LOG(Warning, "coucou"); });
    _actionMap->AddAction(_action);
    _actionMap->AddAction(_action2);
    _actionMap->Enable();
};

bool Game::Update()
{
    M_INPUT.Update(window);
    TM_Seconds& _timer = M_TIMER;
    _timer.Update();
    const float _deltaTime = _timer.GetDeltaTime().asSeconds();
    M_ACTOR.Update(_deltaTime);

    return IsOver();
}

void Game::UpdateWindow()
{
    window.clear();
    M_CAMERA.RenderAllCameras(window);
    window.display();
}

void Game::Stop()
{
    M_ACTOR.BeginDestroy();
}