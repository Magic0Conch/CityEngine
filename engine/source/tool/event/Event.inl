#pragma once
#include "Event.h"
namespace EasyEngine::Tool
{
	template<class... ArgTypes>
	ListenerID Event<ArgTypes...>::addListener(Callback p_callback){
		ListenerID listenerID = m_availableListenerID++;
		m_callbacks.emplace(listenerID, p_callback);
		return listenerID;
	}

	template<class... ArgTypes>
	ListenerID Event<ArgTypes...>::operator+=(Callback p_callback){
		return addListener(p_callback);
	}

	template<class... ArgTypes>
	bool Event<ArgTypes...>::removeListener(ListenerID p_listenerID){
		return m_callbacks.erase(p_listenerID) != 0;
	}

	template<class... ArgTypes>
	bool Event<ArgTypes...>::operator-=(ListenerID p_listenerID){
		return removeListener(p_listenerID);
	}

	template<class... ArgTypes>
	void Event<ArgTypes...>::removeAllListeners(){
		m_callbacks.clear();
	}

	template<class... ArgTypes>
	uint16_t Event<ArgTypes...>::getListenerCount() const{
		return m_callbacks.size();
	}

	template<class... ArgTypes>
	void Event<ArgTypes...>::invoke(ArgTypes... p_args)	{
		for(const auto& [key,func]:m_callbacks){
            func(p_args...);
        }
	}
}