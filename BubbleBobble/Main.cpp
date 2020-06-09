#include "MiniginPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "Minigin.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Components.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "TextObject.h"
#include "Debug.h"
#include "ServiceLocator.h"

void LoadScene()
{
	using namespace divengine;
	//ResourceManager::GetInstance().Init("../Data/");
	ServiceLocator::GetResourceManager().Init("../Data/");

	auto& demoScene = SceneManager::GetInstance().CreateScene("Demo");

	//Add background
	auto go = std::make_shared<GameObject>(Vector3(0.f, 0.f, 0.f));
	RenderComponent* pRenderComp = new RenderComponent();
	pRenderComp->SetTexture("background.jpg");
	go->AddComponent(pRenderComp);
	demoScene.AddObject(go);

	//Add logo dae
	go = std::make_shared<GameObject>(Vector3(216, 180, 0));
	go->AddComponent(new RenderComponent("logo.png"));
	go->AddComponent(new RigidbodyComponent(true));
	ColliderComponent* collider = new BoxColliderComponent(glm::vec2(210, 55), glm::vec2(0, 10));
	go->AddComponent(collider);
	//collider->SetTrigger(true);
	demoScene.AddObject(go);

	auto font = ServiceLocator::GetResourceManager().LoadFont("Lingua.otf", 36);
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_shared<TextObject>("Hello", font);
	go->SetPosition(Vector3(100, 200, 0));
	demoScene.AddObject(go);

	go = std::make_shared<GameObject>(Vector3(216, 0, 0));
	auto rigidBody = new RigidbodyComponent();
	//rigidBody->AddForce(glm::vec2(2, 0));
	go->AddComponent(rigidBody);
	ColliderComponent* colliderCircle = new CircleColliderComponent(50, glm::vec2(50, 50), false, new PhysicsMaterial2D(0.1f, 0.5f));
	//ColliderComponent* collider2 = new BoxColliderComponent(glm::vec2(210, 55), glm::vec2(0,10), false, new PhysicsMaterial2D(0.1f, 0.5f));
	go->SetTriggerCallback([](GameObject* trigger, GameObject* other, GameObject::TriggerFlag flag)
		{
			switch (flag)
			{
			case GameObject::TriggerFlag::enter:
				std::cout << "Entered\n";
				break;

			case GameObject::TriggerFlag::leave:
				std::cout << "Left\n";
				break;

			case GameObject::TriggerFlag::stay:
				std::cout << "Staying\n";
				break;
			}
		});

	//go->AddComponent(collider2);
	go->AddComponent(colliderCircle);
	go->AddComponent(new RenderComponent("logo.png"));
	demoScene.AddObject(go);

	//Add Programming 4 Assignment text
	go = std::make_shared<GameObject>(Vector3(80, 20, 0));
	go->AddComponent(new RenderComponent());
	go->AddComponent(new TextComponent("Programming 4 Assignment", font));
	demoScene.AddObject(go);

	//Add fps counter
	auto FPSCounter = std::make_shared<GameObject>(Vector3(10, 10, 0));
	font = ServiceLocator::GetResourceManager().LoadFont("Lingua.otf", 20);
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	SDL_Color color = { 0, 255, 0 };
	FPSCounter->AddComponent(new RenderComponent());
	FPSCounter->AddComponent(new TextComponent("0", font, color));
	FPSCounter->AddComponent(new FPSComponent());
	demoScene.AddObject(FPSCounter);

	//Test sprite component
	auto sprite = std::make_shared<GameObject>(Vector3(100, 50, 0), 5.f);
	sprite->AddComponent(new SpriteComponent("sprites0.png", 34, 16, 8, 0.006f));
	demoScene.AddObject(sprite);

	SoundManager &sound = ServiceLocator::GetSoundManager();
	sound.LoadSound("Sounds/Apprehensive_at_Best.mp3", "testSound", FMOD_2D | FMOD_LOOP_NORMAL);
	sound.StartSound("testSound");
	sound.SetMasterVolume(0.05f);
}

int main(int, char*[]) {
	divengine::Minigin engine;
	engine.Initialize();
	LoadScene();
	engine.Run();

    return 0;
}