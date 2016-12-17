#include "Timer.h"

namespace SupraHot
{
	namespace Graphics
	{

#ifndef PLATFORM_WINDOWS
		typedef std::chrono::high_resolution_clock Clock;
#endif

		Timer::Timer()
		{
#ifdef PLATFORM_WINDOWS
			// Query for the performance counter frequency
			LARGE_INTEGER largeInt;
			QueryPerformanceFrequency(&largeInt);
			frequency = largeInt.QuadPart;
			frequencyD = static_cast<double>(frequency);

			// Init the elapsed time
			QueryPerformanceCounter(&largeInt);
			startTime = largeInt.QuadPart;
			elapsed = startTime;
#endif

#ifndef PLATFORM_WINDOWS
			StartTime = HighResolutionClock::now();
			ElapsedTime = StartTime;
#endif

		}

		Timer::~Timer()
		{
		}

		void Timer::Update()
		{
#ifdef PLATFORM_WINDOWS
			LARGE_INTEGER largeInt;
			QueryPerformanceCounter(&largeInt);
			int64 currentTime = largeInt.QuadPart - startTime;

			delta = currentTime - elapsed;
			deltaF = static_cast<float>(deltaF);
			deltaSeconds = delta / frequency;
			deltaSecondsD = delta / frequencyD;
			deltaSecondsF = static_cast<float>(deltaSecondsD);
			deltaMillisecondsD = deltaSecondsD * 1000;
			deltaMilliseconds = static_cast<int64>(deltaMillisecondsD);
			deltaMillisecondsF = static_cast<float>(deltaMillisecondsD);
			deltaMicrosecondsD = deltaMillisecondsD * 1000;
			deltaMicroseconds = static_cast<int64>(deltaMicrosecondsD);
			deltaMicrosecondsF = static_cast<float>(deltaMicrosecondsD);

			// Time since the app started
			elapsed = currentTime;
			elapsedF = static_cast<float>(elapsed);
			elapsedSeconds = elapsed / frequency;
			elapsedSecondsD = elapsed / frequencyD;
			elapsedSecondsF = static_cast<float>(elapsedSecondsD);
			elapsedMilliseconds = static_cast<int64>(elapsedSecondsD * 1000);
			elapsedMillisecondsD = elapsedSecondsD * 1000;
			elapsedMillisecondsF = static_cast<float>(elapsedMillisecondsD);
			elapsedMicroseconds = static_cast<int64>(elapsedMillisecondsD * 1000);
			elapsedMicrosecondsD = elapsedMillisecondsD * 1000;
			elapsedMicrosecondsF = static_cast<float>(elapsedMillisecondsD);
#endif

#ifndef PLATFORM_WINDOWS
			std::chrono::time_point<HighResolutionClock> currentTime = HighResolutionClock::now();
			auto delta = currentTime - ElapsedTime;

			deltaSecondsF = std::chrono::duration_cast<milliseconds_type>(delta).count();
			deltaMillisecondsF = std::chrono::duration_cast<milliseconds_type>(delta).count() / 1000.f;

			ElapsedTime = currentTime;
			auto elapsed = currentTime - StartTime;
			elapsedSecondsF = std::chrono::duration_cast<milliseconds_type>(elapsed).count() / 1000.f;
#endif

		}
	}
};
