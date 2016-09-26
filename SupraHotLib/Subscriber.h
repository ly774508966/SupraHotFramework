#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace PubSub
	{
		class Publisher;
		class Subscriber
		{
		public:
			Subscriber();
			virtual ~Subscriber();
			virtual bool OnMessage(std::string message, void* data) = 0;
			void Subscribe(const Publisher& publisher, std::string message);
		};
	};
};
