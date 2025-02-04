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
    Action* _action = new Action("Test", ActionData(KeyPressed, Key::A), [&]() { LOG(Display, "coucou"); });
    Action* _action2 = new Action("Test2", ActionData(KeyReleased, Key::A), [&]() { LOG(Warning, "coucou"); });
    Action* _action3 = new Action("Test3", ActionData(KeyHold, Key::A), [&]() { LOG(Error, "coucou"); });
    Action* _action4 = new Action("Test4", ActionData(KeyPressed, Key::B), [&]() { LOG(Display, "salut"); });
    Action* _action5 = new Action("Test5", ActionData(KeyReleased, Key::B), [&]() { LOG(Warning, "salut"); });
    Action* _action6 = new Action("Test6", ActionData(KeyHold, Key::B), [&]() { LOG(Error, "salut"); });
    _actionMap->AddAction(_action);
    _actionMap->AddAction(_action2);
    _actionMap->AddAction(_action3);
    _actionMap->AddAction(_action4);
    _actionMap->AddAction(_action5);
    _actionMap->AddAction(_action6);
    //TODO y'a un bug qui fait que si on appuie sur une touche 
    // pendant pendant un hold ca fait le hold de la deuxieme
    // avant le pressed

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