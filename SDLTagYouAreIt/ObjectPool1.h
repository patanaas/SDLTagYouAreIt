#ifndef OBJECTPOOL1_H
#define OBJECTPOOL1_H

#include "StandardIncludes.h"

template<class T>
class ObjectPool1
{
public:
	// Constructors and Destructors
	ObjectPool1() { }
	virtual ~ObjectPool1()
	{
		for (unsigned int count = 0; count < m_all.size(); count++)
		{
			cout << "Deleting pool objects: " << typeid(T).name() << endl;
			delete m_all[count];
		}
		m_all.clear();
		m_available.clear();
	}

	// Methods
	T* GetResource()
	{
		if (m_available.empty())
		{
			cout << "Creating new pool object." << typeid(T).name() << endl;
			T* resource = new T();
			m_all.push_back(resource);
			return resource;
		}
		else
		{
			cout << "Reusing existing pool object." << typeid(T).name() << endl;
			T* resource = m_available[0];
			m_available.erase(m_available.begin());
			return resource;
		}
	}

	void ReleaseResource(T* _object)
	{
		//_object->Reset();
		m_available.push_back(_object);
	}

private:
	// Members
	vector<T*> m_available;
	vector<T*> m_all;
};

#endif // !OBJECTPOOL1_H

