#pragma once
#include "pch.h"

namespace uvy {
	class State {
		friend class StateManager;
	public:

		State() : m_isTranscendent(false), m_isTransparent(false), m_name("Base") 
		{}
		virtual ~State() {}

		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		//Setters//

		void SetTransparency(bool l_transparency)	{ m_isTransparent = l_transparency; }
		void SetTranscendency(bool l_transcendency) { m_isTranscendent = l_transcendency; }
		void SetName(const std::string& l_name)		{ m_name = l_name; }

		//Getters//

		bool IsTransparent() const   { return m_isTransparent; }
		bool IsTranscendent() const  { return m_isTranscendent; }
		sf::View& GetView()			 { return m_view; }
		const std::string& GetName() { return m_name; }

	protected:
		std::string m_name;
		bool		m_isTransparent;
		bool		m_isTranscendent;
		sf::View	m_view;
	};
}