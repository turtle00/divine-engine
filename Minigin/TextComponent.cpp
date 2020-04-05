#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
dae::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color)
	:m_Text{text}
	,m_Font{font}
	,m_NeedsUpdate{true}
	,m_Color{color}
{

}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::Update(float MsPerUpdate)
{
	UNREFERENCED_PARAMETER(MsPerUpdate);
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		const auto renderComp = m_pGameObject->GetComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->SetTexture(texture);
		}
		else
		{
			throw std::exception("TextComponent::Update(): Could not find RenderComponent");
		}
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render()
{

}
