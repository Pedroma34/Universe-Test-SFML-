#pragma once

#include "pch.h"
#include "States/StateGame.h"

namespace uvy {
	class StateManager {
	public:
		StateManager();
		~StateManager();

		void Update();
		void Render();
		void LateUpdate();

		template<class T>
		void SwitchTo();

		template<class T>
		void Remove();

		//Getters//

		std::weak_ptr<State> GetState(const std::string& l_stateName);

	private:
		template<class T>
		std::weak_ptr<T> Create();

	private:
		std::vector<std::shared_ptr<State>> m_states;
		std::vector<std::weak_ptr<State>>	m_toRemove;
	};


	template<class T>
	inline void StateManager::SwitchTo() {
		for (auto itr = m_states.begin(); itr != m_states.end(); itr++)
			if (typeid(*itr->get()) == typeid(T)) {								//If already in memory

				m_states.back()->Deactivate();									//Deactivating current state since we're pushing one to the back
				std::shared_ptr<State> tmpState = std::move(*itr);				//Passing ownership to a temporary state ref because we're deleting it from container
				m_states.erase(itr);											//Deleting from stack, but it's store in the temporary variable
				tmpState->Activate();
				m_states.push_back(std::move(tmpState));					    //Pushing and passing ownership to the container.
				SharedData::GetWindow().SetView(m_states.back()->GetView());	//Setting view
				return;
			}

		//Executing code if state is not in container
		if (!m_states.empty())
			m_states.back()->Deactivate();		//Checking if there's a state in the back of container and deactivating it

		std::shared_ptr<T> newState = Create<T>().lock();   //Creating new state
		newState->Activate();
		SharedData::GetWindow().SetView(m_states.back()->GetView());
	}


	template<class T>
	inline void StateManager::Remove() {
		for (auto& itr : m_states)
			if (typeid(*itr) == typeid(T)) {
				m_toRemove.push_back(itr);		//Pusing weak pointer to container. @see m_toRemove
			}
	}


	template<class T>
	inline std::weak_ptr<T> StateManager::Create() {

		std::shared_ptr<State> newState = std::make_shared<T>();
		newState->m_view = SharedData::GetWindow().GetDefaultView();
		newState->OnCreate();
		m_states.push_back(std::move(newState));  //Moving state and ownership to the container

		return std::static_pointer_cast<T>(m_states.back());
	}
}