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
    //DIGITAL
    _actionMap->AddAction("Test", ActionData(MouseEntered), [&]() { LOG(Error, "ButtonEntered"); });
    _actionMap->AddAction("Test1", ActionData(KeyPressed, KeyType::A), [&]() { LOG(Display, "coucou"); });
    _actionMap->AddAction("Test2", ActionData(KeyReleased, KeyType::A), [&]() { LOG(Warning, "coucou"); });
    _actionMap->AddAction("Test3", ActionData(KeyHold, KeyType::A), [&]() { LOG(Error, "coucou"); });
    _actionMap->AddAction("Test4", ActionData(KeyPressed, KeyType::B), [&]() { LOG(Display, "salut"); });
    _actionMap->AddAction("Test5", ActionData(KeyReleased, KeyType::B), [&]() { LOG(Warning, "salut"); });
    _actionMap->AddAction("Test6", ActionData(KeyHold, KeyType::B), [&]() { LOG(Error, "salut"); });
    _actionMap->AddAction("Test9", ActionData(KeyHold, KeyType::T), [&]() { LOG(Error, "Thomas"); });
    _actionMap->AddAction("Test8", ActionData(KeyReleased, KeyType::T), [&]() { LOG(Warning, "Thomas"); });
    _actionMap->AddAction("Test7", ActionData(KeyPressed, KeyType::T), [&]() { LOG(Display, "Thomas"); });
    //AXIS2
    _actionMap->AddAction("Test10", ActionData(MouseMoved), [&](const Vector2f& _position) { LOG(Error, "X: " + to_string(_position.x) + " Y: " + to_string(_position.y)); });
    _actionMap->AddAction("Test11", ActionData(MouseMovedRaw), [&](const Vector2f& _position) { LOG(Error, "MouseMovedRaw"); });
    //AXIS
    _actionMap->AddAction("Test12", ActionData(MouseWheelScrolled, KeyType::Vertical), [&](const float _position) { LOG(Error, to_string(_position)); });
    _actionMap->AddAction("Test99", ActionData(JoystickMoved, 0, KeyType::LeftJoystickX), [&](const float _position) { LOG(Error, "manette :" + to_string(_position)); });

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