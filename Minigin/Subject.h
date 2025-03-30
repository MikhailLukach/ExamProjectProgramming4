#pragma once
#include <vector>
#include <algorithm>
#include "Observer.h"

namespace dae
{
    class Subject
    {
    public:
        //should not use the shared_ptr, it should not have ownership over an observer, just raw pointers
        void AddObserver(std::shared_ptr<Observer> observer)
        {
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
                if (auto observer = it->lock())
                {
                    std::cout << "[DEBUG] Notifying an observer!\n";
                    observer->Notify(event, gameObject);
                    ++it;
                }
            }
        }

    private:
        std::vector<std::weak_ptr<Observer>> m_Observers;
    };
}