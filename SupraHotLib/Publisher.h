#pragma once
#include "Platform.h"
#include "Subscriber.h"
#include <vector>
#include <map>

namespace SupraHot
{
	namespace PubSub
	{
		class Publisher
		{
		private:
			std::map<std::string, std::vector<const Subscriber*>> Subscribers;
		public:
			Publisher();
			~Publisher();
			void Publish(const std::string& message, void* data);
			void AddSubscriber(const Subscriber* subscriber, std::string message);
			void RemoveSubscriber(const Subscriber* subscriber, std::string message);
			void Delete();

			static Publisher& GetSystemPublisher();
		};
	};
};


