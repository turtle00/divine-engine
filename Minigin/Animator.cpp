#include "MiniginPCH.h"
#include "Animator.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Time.h"
#include "GameObject.h"

divengine::Animator::Animator(const std::string& path, float clipHeight, float clipWidth)
	:m_pAnimations{},
	m_AnimationSpeed{1.0f}
	,m_IsPlaying{false}
	,m_pCurrentAnimation(nullptr)
	,m_Path{path}
	,m_Cols{0}
	,m_Rows{0}
	,m_CurrentFrame{}
	,m_CurrentFrameTime{}
	,m_DestRect{}
	,m_SrcRect{}
	,m_pTransform{nullptr}
	, m_ClipHeight{ clipHeight }
	, m_ClipWidth{ clipWidth }
{
	m_SrcRect.w = int(clipWidth);
	m_SrcRect.h = int(clipHeight);
}

divengine::Animator::~Animator()
{
	for (auto pAnim : m_pAnimations)
	{
		SAFEDELETE(pAnim);
	}
}

void divengine::Animator::SetAnimation(AnimationClip* animation)
{
	//Set current clip
	m_pCurrentAnimation = animation;
}

void divengine::Animator::SetAnimation(const std::string& name)
{
	//Set current clip
	std::vector<AnimationClip*>::iterator it = std::find_if(m_pAnimations.begin(), m_pAnimations.end(), [name](AnimationClip* clip) {return (clip->m_Name == name); });
	if (it != m_pAnimations.end())
	{
		SetAnimation(*it);
	}
	else
	{
		Debug::LogWarning("Animator::SetAnimation: no animations exists with name ", name.c_str(), "!");
	}
}

void divengine::Animator::AddAnimation(AnimationClip *animation)
{
	m_pAnimations.push_back(animation);
	if (!m_pCurrentAnimation)
		m_pCurrentAnimation = animation;
}

void divengine::Animator::Update()
{
	//Update current clip
	if (!m_IsPlaying || !m_pCurrentAnimation)
		return;
	
	//Update frame
	m_CurrentFrameTime += Time::GetInstance().GetDeltaTime() * m_AnimationSpeed;
	if (m_CurrentFrameTime >= (1.f / m_pCurrentAnimation->m_FramesPerSecond))
	{
		++m_CurrentFrame;
		m_CurrentFrameTime = 0.f;
	}
	m_CurrentFrame %= m_pCurrentAnimation->m_NrOfFrames;

	//index = (row * cols) + col
	int col = ( m_CurrentFrame + m_pCurrentAnimation->m_StartCol) % (m_Cols);
	int row = ((m_pCurrentAnimation->m_StartRow + m_CurrentFrame + m_pCurrentAnimation->m_StartCol - col) / (m_Cols)) % m_Rows;

	m_SrcRect.x =  int((m_ClipWidth) * col);
	m_SrcRect.y = (int(m_ClipHeight) * row); 

	//Update dest rect
	auto pos = m_pTransform->GetPosition();
	m_DestRect.x = (int)pos.x;
	m_DestRect.y = (int)pos.y;
	
	m_pRenderComp->SetSourceRect(m_SrcRect);
	m_pRenderComp->SetDestRect(m_DestRect);
}

void divengine::Animator::Initialize()
{
	m_pRenderComp = new RenderComponent(m_Path);
	m_pGameObject->AddComponent(m_pRenderComp);

	m_pTransform = m_pGameObject->GetComponent<TransformComponent>();
	if (!m_pTransform)
	{
		m_pTransform = new TransformComponent(Vector3());
		m_pGameObject->AddComponent(m_pTransform);
		Debug::LogWarning("Animator::Initialize: no transform component found");
	}
	
	m_pRenderComp->SetSourceRect(m_SrcRect);

	auto dimensions = m_pRenderComp->GetTextureDimensions();
	m_Cols = int(dimensions.x / m_ClipWidth);
	m_Rows = int(dimensions.y / m_ClipHeight);

	m_DestRect.x = (int)m_pTransform->GetPosition().x;
	m_DestRect.y = (int)m_pTransform->GetPosition().y;

	m_DestRect.w = int(m_SrcRect.w);
	m_DestRect.h = int(m_SrcRect.h);
	m_pRenderComp->SetDestRect(m_DestRect);
}

void divengine::Animator::Render()
{

}

void divengine::Animator::Restart()
{
	//Reset current clip
	m_IsPlaying = true;
	m_AnimationSpeed = 1.0f;
	m_CurrentFrameTime = 0;
	m_CurrentFrame = 0;
	//Restart current clip
}

void divengine::Animator::Play()
{
	//Play/start current clip
	m_IsPlaying = true;
}

void divengine::Animator::Pause()
{
	m_IsPlaying = false;
}