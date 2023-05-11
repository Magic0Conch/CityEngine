#pragma once

namespace EasyEngine {
	class Driver
	{
	public:
		Driver();
		~Driver() = default;

		bool isActive() const;

	private:
		void initGlad();
		
	private:
		bool m_isActive;
	};
}