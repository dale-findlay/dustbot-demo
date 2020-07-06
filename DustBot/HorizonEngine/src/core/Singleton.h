#pragma once

namespace Horizon
{
	template<typename T>
	class Singleton
	{
	protected:
		Singleton()
		{}

		static T* m_instance;

	public:
		static T* CreateInstance()
		{
			m_instance = new T();
			return m_instance;
		}

		static T* GetInstance()
		{
			return m_instance;
		}
	};

	template<typename T>
	T* Singleton<T>::m_instance = nullptr;

}