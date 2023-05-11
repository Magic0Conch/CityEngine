#pragma once
#include <functional>
#include <stdint.h>
#include <unordered_map>
namespace EasyEngine::Tool {
    using ListenerID = uint64_t;

    template<class... ArgTypes>
    class Event{
    public:
        using Callback = std::function<void(ArgTypes...)>;

        ListenerID addListener(Callback p_callback);
        ListenerID operator+=(Callback p_callback);
        bool removeListener(ListenerID p_listenerID);
        bool operator-=(ListenerID p_listenerID);

        void removeAllListeners();
        uint16_t getListenerCount() const;
        void invoke(ArgTypes... p_args);
    private:
        std::unordered_map<ListenerID, Callback> m_callbacks;
        ListenerID m_availableListenerID = 0;
    };
}
#include "Event.inl"