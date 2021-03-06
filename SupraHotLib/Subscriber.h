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
			void Subscribe(Publisher& publisher, std::string message);
			void Unsubscribe(Publisher& publisher, std::string message);
		};
	};
};
