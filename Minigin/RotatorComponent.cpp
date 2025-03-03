#include "RotatorComponent.h"
#include "GameObject.h"
#include "Transform.h"

dae::RotatorComponent::RotatorComponent(float radius, float speed, const glm::vec3& center)
	:m_angle(0.0f), m_radius(radius), m_speed(speed), m_center(center)
{
}

void dae::RotatorComponent::Update(float deltaTime)
{
    //GameObject* owner = GetOwner();
    //owner will always exist get rid of it
    //a sharedptr is not necessary, don't make one
    /*std::shared_ptr<GameObject> parent = GetOwner()->GetParent();
    if (parent)
    {
        center = parent->GetWorldPosition();
    }*/
    Transform* transform = GetOwner()->GetTransform().get();
    glm::vec3 center = m_center;
    if(GetOwner()->GetParent())
    {
        center = GetOwner()->GetParent()->GetTransform()->GetWorldPosition();
    }

    /*m_angle += m_speed * deltaTime;
    if (m_angle >= 360.f)
        m_angle -= 360.f;
    else if (m_angle <= 360.f)
        m_angle += 360.f;*/

    m_angle = fmod(m_angle + (m_speed * deltaTime), 360.0f);

    float radians = glm::radians(m_angle);
    float x = center.x + m_radius * cos(radians);
    float y = center.y + m_radius * sin(radians);

    transform->SetPosition(x, y, 0.0f);
}

void dae::RotatorComponent::SetRadius(float radius)
{
    m_radius = radius;
}

void dae::RotatorComponent::SetSpeed(float speed)
{
    m_speed = speed;
}
