#include "FileManager.h"
#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "Components.h"
#include "MenuController.h"

void FileManager::LoadLevel(const std::string& file, const std::string& sceneName) //Read from file
{
	using namespace divengine;

	auto scene = SceneManager::GetInstance().GetSceneByName(sceneName);
	if (!scene)
		return;

	//Open binary stream	
	m_Reader.Open(file);
	if (!m_Reader.isOpen())
		return;

	//Read amount of gameobjects
	size_t objectCount = 0;
	m_Reader.Read(objectCount);

	//start reading all objects:
	for (size_t i = 0; i < objectCount; i++)
	{
		GameObject* pObject = new GameObject();
		pObject->Load(m_Reader);
		//read component count for the object
		size_t componentCount = 0;
		m_Reader.Read(componentCount);

		for (size_t j = 0; j < componentCount; j++)
		{
			unsigned int componentType = 0;
			m_Reader.Read(componentType);

			//Create component according to type
			BaseComponent* pComponent = nullptr;
			if (componentType >= int(ComponentType::TYPECOUNT))
			{
				//Load Game components
				GameComponentType gameCompType = GameComponentType(componentType);
				switch (gameCompType)
				{
				case GameComponentType::playercontroller:
					pComponent = new PlayerController();
					break;

				case GameComponentType::menucontroller:
					pComponent = new MenuController();
					break;
				}
			}
			else
			{
				//Load engine components
				ComponentType engineCompType = ComponentType(componentType);
				switch (engineCompType)
				{
				case ComponentType::transformcomponent:
					pComponent = pObject->GetComponent<TransformComponent>();
					break;

				case ComponentType::rendercomponent:
					pComponent = new RenderComponent();
					break;

				case ComponentType::rigidbodycomponent:
					pComponent = new RigidbodyComponent();
					break;

				case ComponentType::collidercomponent:
					//Means collider was not set
					Debug::Log("FileManager::Load: Collider type was not set, cannot add");
					break;

				case ComponentType::boxcollider:
					pComponent = new BoxColliderComponent();
					break;

				case ComponentType::animator:
					
					break;

				case ComponentType::platformcollider:
					pComponent = new PlatformColliderComponent(glm::vec2());
					break;

				case ComponentType::unknown:
					break;
				}
			}
			if (pComponent)
			{
				//Load the component
				pObject->AddComponent(pComponent);
				pComponent->Load(m_Reader);
				//add it to the gameObject
			}
		}

		//Add this to the scene
		scene->AddObject(pObject);
	}

	//close stream
	m_Reader.Close();
}

void FileManager::SaveLevel(const std::string& file, const std::string& levelname)//Write to file
{
	using namespace divengine;

	//open stream
	m_Writer.Open(file);

	//save gameObject count (from the current scene)
	auto scene = SceneManager::GetInstance().GetSceneByName(levelname);
	if (!scene)
	{
		m_Writer.Close();
		return;
	}
	auto objects = scene->GameObjects();
	size_t objectCount = objects.size();
	m_Writer.Write(objectCount);

	//Start saving the objects:
	for (size_t i = 0; i < objectCount; i++)
	{
		objects[i]->Save(m_Writer);
		auto components = objects[i]->Components();
		//save component count per gameobject (from gameobject itself)
		m_Writer.Write(components.size());

		for (size_t j = 0; j < components.size(); j++)
		{
			//save component type
			m_Writer.Write(components[j]->Type());
			components[j]->Save(m_Writer); //call save components per component
		}
	}

	//close stream
	m_Writer.Close();
}

void FileManager::ReloadLevel(const std::string& file, const std::string& name)
{
	using namespace divengine;

	//open stream
	m_Reader.Open(file);

	//read gameObject count (from the current scene)
	auto scene = SceneManager::GetInstance().GetSceneByName(name);
	if (!scene)
	{
		m_Writer.Close();
		return;
	}
	auto objects = scene->GameObjects();
	size_t objectCount = objects.size();
	m_Reader.Read(objectCount);

	//Start reloading the objects:
	for (size_t i = 0; i < objectCount; i++)
	{
		objects[i]->Load(m_Reader);
		auto components = objects[i]->Components();
		//read component count per gameobject (from gameobject itself)
		size_t componentCount = 0;
		m_Reader.Read(componentCount);
		for (size_t j = 0; j < componentCount; j++)
		{
			//read component type
			unsigned int componentType = 0;
			m_Reader.Read(componentType);

			components[j]->Load(m_Reader); //call load components per component
		}
	}

	//close stream
	m_Reader.Close();
}

FileManager::FileManager()
{
}
