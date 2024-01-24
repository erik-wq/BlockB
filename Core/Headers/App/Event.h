#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

template<typename T>
class EventManager {
public:
    // Define a type for the event handler function with multiple arguments
    using Handle = std::function<void(T)>;

    // Subscribe a function to the event
    void subscribe(const Handle& handler);

    // Unsubscribe a function from the event
    void unsubscribe(const Handle& handler);

    // Trigger the event, calling all subscribed functions
    void triggerEvent(const T args);

private:
    // Vector to store the subscribed event handlers
    std::vector<Handle> eventHandlers;
};

template<typename T>
inline void EventManager<T>::subscribe(const Handle& handler)
{
    eventHandlers.push_back(handler);
}

template<typename T>
inline void EventManager<T>::unsubscribe(const Handle& handler)
{
    eventHandlers.erase(std::remove_if(eventHandlers.begin(), eventHandlers.end(),
        [&handler](const Handle& h) { return h.target_type() == handler.target_type(); }),
        eventHandlers.end());
}

template<typename T>
inline void EventManager<T>::triggerEvent(const T args)
{
    for (const auto& handler : eventHandlers) {
        handler(args);
    }
}