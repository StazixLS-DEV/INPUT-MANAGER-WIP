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
    _actionMap->AddAction("Test", ActionData(MouseEntered), [&]() { LOG(Error, "ButtonEntered"); });
    _actionMap->AddAction("Test1", ActionData(KeyPressed, Key::A), [&]() { LOG(Display, "coucou"); });
    _actionMap->AddAction("Test2", ActionData(KeyReleased, Key::A), [&]() { LOG(Warning, "coucou"); });
    _actionMap->AddAction("Test3", ActionData(KeyHold, Key::A), [&]() { LOG(Error, "coucou"); });
    _actionMap->AddAction("Test4", ActionData(KeyPressed, Key::B), [&]() { LOG(Display, "salut"); });
    _actionMap->AddAction("Test5", ActionData(KeyReleased, Key::B), [&]() { LOG(Warning, "salut"); });
    _actionMap->AddAction("Test6", ActionData(KeyHold, Key::B), [&]() { LOG(Error, "salut"); });
    _actionMap->AddAction("Test7", ActionData(KeyPressed, Key::T), [&]() { LOG(Display, "Thomas"); });
    _actionMap->AddAction("Test8", ActionData(KeyReleased, Key::T), [&]() { LOG(Warning, "Thomas"); });
    _actionMap->AddAction("Test9", ActionData(KeyHold, Key::T), [&]() { LOG(Error, "Thomas"); });
    _actionMap->AddAction("Test10", ActionData(MouseMoved), [&]() { LOG(Error, "MouseMoved"); });
    _actionMap->AddAction("Test11", ActionData(MouseMovedRaw), [&]() { LOG(Error, "MouseMovedRaw"); });
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