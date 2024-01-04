#include "pch.h"
#include "StateManager.h"

uvy::StateManager::StateManager() {
	SharedData::SetStateManager(this);
}

uvy::StateManager::~StateManager() {}

void uvy::StateManager::Update() {
	if (m_states.empty())
		return;

	if (m_states.back()->IsTranscendent() && m_states.size() > 1) { //If current state is transcendent, we will update the one behind it

		auto lastState = m_states.end();

		while (lastState != m_states.begin()) {

			if (lastState != m_states.end())
				if (!lastState->get()->IsTranscendent())
					break;

			--lastState;
		}
		for (; lastState != m_states.end(); ++lastState)
			lastState->get()->Update();
	}

	else //If current state is not transcendent, just update it
		m_states.back()->Update();
}

void uvy::StateManager::Render() {
	if (m_states.empty())
		return;

	if (m_states.back()->IsTransparent() && m_states.size() > 1) { //If current state is transparent, we will render the one behind it
	
			auto lastState = m_states.end();
	
			while (lastState != m_states.begin()) {

			if (lastState != m_states.end())
				if (!lastState->get()->IsTransparent())
					break;

			--lastState;
		}
			for (; lastState != m_states.end(); ++lastState) {
				SharedData::GetWindow().SetView(lastState->get()->GetView());
				lastState->get()->Draw();
			}
		}

	else { //If current state is not transparent, just render it
		SharedData::GetWindow().SetView(m_states.back()->GetView());
		m_states.back()->Draw();
	}
}

void uvy::StateManager::LateUpdate() {
	while (m_toRemove.begin() != m_toRemove.end()) {

		for (auto itr = m_states.begin(); itr != m_states.end(); itr++) {

			if (typeid(**itr) == typeid(*m_toRemove.begin()->lock())) {
				itr->get()->OnDestroy();
				m_states.erase(itr);
				break;
			}
		}

		m_toRemove.erase(m_toRemove.begin());
	}
}

std::weak_ptr<uvy::State> uvy::StateManager::GetState(const std::string& l_stateName) {
	for(auto& state : m_states)
		if(state->GetName() == l_stateName)
			return state;

	return std::weak_ptr<State>();
}
