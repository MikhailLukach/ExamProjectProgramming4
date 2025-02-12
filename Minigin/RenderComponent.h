#pragma once
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include <memory>

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		explicit RenderComponent(const std::string& texturePath = "");
		void Render() const override;
		void SetTexture(const std::string& texturePath);
		void SetPosition(float x, float y);
	private:
		std::shared_ptr<Texture2D> m_texture;
		std::weak_ptr<Transform> m_transform;
		float m_x{ 0 }, m_y{ 0 };
	};
}


