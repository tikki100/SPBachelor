#include <iostream>
#include <chrono>

//Code by The Cherno
//https://www.youtube.com/watch?v=YG4jexlSAjc
namespace Eng
{
class Timer
{
public:
	Timer()
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		Stop();
	}

	void Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast< std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast< std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		std::cout << duration << "us (" << ms << "ms)" << std::endl;
	}
private:
	std::chrono::time_point< std::chrono::high_resolution_clock > m_StartTimepoint;

};//End of class Timer
}//End of namespace Eng
