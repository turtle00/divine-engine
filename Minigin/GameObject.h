#pragma once
#include "Structs.h"
#include <functional>
namespace divengine
{
	class BaseComponent;
	class TransformComponent;

	//class Texture2D;
	class GameObject
	{
	public:
		enum class TriggerFlag
		{
			enter,
			leave,
			stay
		};

		typedef std::function<void(GameObject * trigger, GameObject * other, TriggerFlag action)> TriggerCallback;

		virtual void Update();
		virtual void Render() const;

		void SetPosition(float x, float y, float z = 0.f);
		void SetPosition(const Vector3& newPos);
		Vector3 GetPosition() const;
	
		void SetTag(const std::string& tag) { m_Tag = tag; };
		const std::string& GetTag() const { return m_Tag; };

		void SetActive(bool isActive) { m_IsActive = isActive; };

		void AddComponent(BaseComponent* pComponent);
		void RemoveComponent(BaseComponent* pComponent);
		void OnTrigger(GameObject* trigger, GameObject* other, TriggerFlag flag);
		void SetTriggerCallback(TriggerCallback callback);
		void Initialize();

		GameObject(const Vector3& position = Vector3(), float scale = 1);
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

#pragma region Template functions
		//Template functions
		template<class T>
		T* GetComponent()
		{
			//Find first component of type T
			const auto& type = typeid(T);
			for (BaseComponent* pComponent : m_pComponents)
			{
				if (pComponent && (typeid(*pComponent) == type))
				{
					return static_cast<T*>(pComponent);
				}
			}
			return nullptr;
		}

		template<class T>
		std::vector<T*> GetComponents()
		{
			const auto& type = typeid(T);
			std::vector<T*> components;
			//Get all components of type T
			for (BaseComponent* pComponent : m_pComponents)
			{
				if (pComponent && (typeid(*pComponent) == type))
				{
					components.push_back(static_cast<T*>(pComponent));
				}
			}
			return components;
		}
#pragma endregion

	private:
		std::vector<BaseComponent*> m_pComponents;
		TriggerCallback m_TriggerCallback;
		TransformComponent* m_Transform; //a lot of GameObjects have a position, having it here will be more efficient
		std::string m_Tag;
		bool m_IsActive;
	};
}
