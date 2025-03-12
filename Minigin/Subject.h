#pragma once
#include <vector>
#include <algorithm>
#include "Observer.h"

namespace dae
{
    class Subject
    {
    public:
        void AddObserver(std::shared_ptr<Observer> observer)
        {
            // Store weak_ptr instead of raw pointer
            m_Observers.push_back(observer);
            std::cout << "[DEBUG] Total Observers: " << m_Observers.size() << std::endl;
        }

        void RemoveObserver(std::shared_ptr<Observer> observer)
        {
            m_Observers.erase(
                std::remove_if(m_Observers.begin(), m_Observers.end(),
                    [&](const std::weak_ptr<Observer>& weakObserver)
                    {
                        auto lockedObserver = weakObserver.lock();
                        return !lockedObserver || lockedObserver == observer;
                    }),
                m_Observers.end());
        }

    protected:
        void NotifyObservers(EventId event, GameObject* gameObject)
        {
            for (auto it = m_Observers.begin(); it != m_Observers.end();)
            {
                if (auto observer = it->lock()) // Convert weak_ptr to shared_ptr safely
                {
                    std::cout << "[DEBUG] Notifying an observer!\n";
                    observer->Notify(event, gameObject);
                    ++it;
                }
                else
                {
                    std::cout << "[DEBUG] WARNING: Null observer found!\n";
                    it = m_Observers.erase(it); // Remove expired observer
                }
            }
        }

    private:
        std::vector<std::weak_ptr<Observer>> m_Observers; // Store weak_ptr instead of raw pointers
    };
}