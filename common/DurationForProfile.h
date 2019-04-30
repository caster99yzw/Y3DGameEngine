#pragma once
#include <chrono>

namespace y3dcommon
{
	class DurationForProfile
	{
	public:
		DurationForProfile()
		{
			startTime = endTime = std::chrono::high_resolution_clock::now();
		}

		DurationForProfile(DurationForProfile const&) = delete;
		DurationForProfile& operator= (DurationForProfile const&) = delete;
		DurationForProfile(DurationForProfile &&) = default;
		DurationForProfile& operator= (DurationForProfile &&) = default;

		void Stop()
		{
			if (startTime == endTime)
				endTime = std::chrono::high_resolution_clock::now();
		}

		void Reset()
		{
			if (startTime != endTime)
				startTime = endTime = std::chrono::high_resolution_clock::now();
		}

		// Returns the elapsed time in nanoseconds since the profiler was started.
		double Duration_Nanoseconds() const
		{
			return std::chrono::duration<double, std::chrono::nanoseconds::period>(endTime - startTime).count();
		}

		// Returns the elapsed time in microseconds since the profiler was started.
		double Duration_Microseconds() const
		{
			return std::chrono::duration<double, std::chrono::microseconds::period>(endTime - startTime).count();
		}

		// Returns the elapsed time in milliseconds since the profiler was started.
		double Duration_Milliseconds() const
		{
			return std::chrono::duration<double, std::chrono::milliseconds::period>(endTime - startTime).count();
		}

		// Returns the elapsed time in seconds since the profiler was started.
		double Duration_Seconds() const
		{
			return std::chrono::duration<double, std::chrono::seconds::period>(endTime - startTime).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock>		startTime;
		std::chrono::time_point<std::chrono::high_resolution_clock>		endTime;
	};
}

