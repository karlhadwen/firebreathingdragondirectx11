#ifndef UPDATER_H
#define UPDATER_H

#include "stdafx.h"

// add something to the que within IUpdateable, such as an item and update the vector that needs updating
class Updater {
public:
	vector<IUpdateable*> m_Updateables;
	vector<IUpdateable*> m_Waiting;
	vector<IUpdateable*> m_Purge;
	bool Updating;

	void AddUpdatable(IUpdateable& item) {
		if (Updating) {
			m_Waiting.push_back(&item);
		} else {
			m_Updateables.push_back(&item);
		}
		
	}

	void RemoveUpdatable(IUpdateable& item) {
		if (Updating) {
			m_Purge.push_back(&item);
		} else {
			removeFromArray(m_Updateables, &item);
		}
	}

	void Update() {
		Updating = true;

		for (vector<IUpdateable*>::iterator i = m_Updateables.begin(); i != m_Updateables.end(); ++i) {
			(*i)->Update();
		}
		Updating = false;

		if (m_Waiting.size() > 0) {
			for (int i = 0; i < m_Waiting.size();i++) {
				m_Updateables.push_back(m_Waiting[i]);
			}
			m_Waiting.clear();
		}

		if (m_Purge.size() > 0) {
			for (int i = 0; i < m_Purge.size();i++) {
				removeFromArray(m_Updateables, m_Purge[i]);
			}
			m_Purge.clear();
		}
	}
};

#endif
