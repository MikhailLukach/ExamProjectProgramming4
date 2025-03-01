#include "RotatorComponent.h"
#include "GameObject.h"
#include "Transform.h"

dae::RotatorComponent::RotatorComponent(std::shared_ptr<Transform> transform, float radius, float speed
	, const glm::vec3& center)
	:m_pTransform(transform), m_center(center), m_angle(0.0f), m_radius(radius), m_speed(speed)
{
	if (!m_pTransform)
	{
		std::cerr << "RotatorComponent requires a valid TransformComponent!\n";
	}
}

void dae::RotatorComponent::Update(float deltaTime)
{
    if (!m_pTransform)
        return;
    glm::vec3 center = m_center;
    GameObject* owner = GetOwner();
    //owner will always exist get rid of it
    //a sharedptr is not necessary, don't make one
    /*std::shared_ptr<GameObject> parent = GetOwner()->GetParent();
    if (parent)
    {
        center = parent->GetWorldPosition();
    }*/
    if(owner->GetParent())
    {
        center = owner->GetParent()->GetTransform()->GetWorldPosition();
    }

    m_angle += m_speed * deltaTime;
    if (m_angle >= 360.f)
        m_angle -= 360.f;
    else if (m_angle <= 360.f)
        m_angle += 360.f;

    float radians = glm::radians(m_angle);
    float x = center.x + m_radius * cos(radians);
    float y = center.y + m_radius * sin(radians);

    m_pTransform->SetPosition(x, y, 0.0f);
}

void dae::RotatorComponent::SetCenter(const glm::vec3& newCenter)
{
    m_center = newCenter;
}

void dae::RotatorComponent::SetRadius(float radius)
{
    m_radius = radius;
}

void dae::RotatorComponent::SetSpeed(float speed)
{
    m_speed = speed;
}
