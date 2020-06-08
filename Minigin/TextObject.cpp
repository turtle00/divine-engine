#include "MiniginPCH.h"
#include "SDL_ttf.h"
#include "TextObject.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

divengine::TextObject::TextObject(const std::string& text, const std::shared_ptr<Font>& font) 
	: m_NeedsUpdate(true), m_Text(text), m_Font(font), m_Texture(nullptr)
{ }

void divengine::TextObject::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
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
		m_Texture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void divengine::TextObject::Render() const
{
	if (m_Texture != nullptr)
	{	
		const auto pos = GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void divengine::TextObject::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
