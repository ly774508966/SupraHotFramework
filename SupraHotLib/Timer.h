#pragma once
#include "Platform.h"
#include <iostream>

#ifndef PLATFORM_WINDOWS
	#include <chrono>
#endif

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <commctrl.h>
	#include <psapi.h>
	#include <process.h>
#endif

namespace SupraHot
{
	namespace Graphics
	{
		class Timer
		{

		public:
			Timer();
			~Timer();

			void Update();

		protected:

#ifndef PLATFORM_WINDOWS
			typedef std::chrono::high_resolution_clock HighResolutionClock;
			typedef std::chrono::duration<float, std::milli> milliseconds_type;

			std::chrono::time_point<HighResolutionClock> StartTime;
			std::chrono::time_point<HighResolutionClock> ElapsedTime;
			std::chrono::time_point<HighResolutionClock> DeltaTime;
#endif

			float deltaMillisecondsF;
			float deltaSecondsF;
			float elapsedSecondsF;

#ifdef PLATFORM_WINDOWS
			int64 startTime;

			int64 frequency;
			double frequencyD;

			int64 elapsed;
			int64 delta;

			float elapsedF;
			float deltaF;

			double elapsedD;
			double deltaD;

			int64 elapsedSeconds;
			int64 deltaSeconds;

			double elapsedSecondsD;
			double deltaSecondsD;

			int64 elapsedMilliseconds;
			int64 deltaMilliseconds;

			float elapsedMillisecondsF;

			double elapsedMillisecondsD;
			double deltaMillisecondsD;

			int64 elapsedMicroseconds;
			int64 deltaMicroseconds;

			float elapsedMicrosecondsF;
			float deltaMicrosecondsF;

			double elapsedMicrosecondsD;
			double deltaMicrosecondsD;
#endif
		public:
			float DeltaMillisecondsF()
			{
				return deltaMillisecondsF;
			};

			float DeltaSecondsF()
			{
				return deltaSecondsF;
			};

			float ElapsedSecondsF()
			{
				return elapsedSecondsF;
			};

		};
	};
};