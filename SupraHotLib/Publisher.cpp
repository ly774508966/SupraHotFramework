#include "Publisher.h"
#include <algorithm>

namespace SupraHot
{
	namespace PubSub
	{
		Publisher::Publisher()
		{
			
		}

		Publisher::~Publisher()
		{
		}

		void Publisher::Publish(const std::string& message, void* data)
		{
			if (Subscribers.find(message) != Subscribers.end())
			{
				std::vector<const Subscriber*>& subscriberVector = Subscribers.at(message);

				for (uint32 i = 0, l = static_cast<uint32>(subscriberVector.size()); i < l; ++i)
				{
					Subscriber* sub = const_cast<Subscriber*>(subscriberVector.at(i));
					if (sub->OnMessage(message, data))
					{
						return;
					}
				}
			}
		}

		void Publisher::AddSubscriber(const Subscriber* subscriber, std::string message)
		{
			// Create new vector, if none exists for this message
			if (Subscribers.find(message) == Subscribers.end())
			{
				Subscribers[message] = std::vector<const Subscriber*>();
			}

			std::vector<const Subscriber*>& subscriberVector = Subscribers.at(message);

			for (uint32 i = 0, l = static_cast<uint32>(subscriberVector.size()); i < l; ++i)
			{
				const Subscriber* sub = subscriberVector.at(i);
				if (sub == subscriber)
				{
					// Subscriber already get notyfied for this message
					return;
				}
			}

			subscriberVector.push_back(subscriber);
		}

		void Publisher::RemoveSubscriber(const Subscriber* subscriber, std::string message)
		{
			if (Subscribers.find(message) != Subscribers.end())
			{
				std::vector<const Subscriber*>& subscriberVector = Subscribers.at(message);
				subscriberVector.erase(std::remove(subscriberVector.begin(), subscriberVector.end(), subscriber), subscriberVector.end());
			}
		}

		void Publisher::Delete()
		{
			typedef std::map<std::string, std::vector<const Subscriber*>>::iterator it_type;
			for (it_type iterator = Subscribers.begin(); iterator != Subscribers.end(); ++iterator) {
				// Delete all subscriber vectors
				Subscribers.clear();
			}
		}

		Publisher& Publisher::GetSystemPublisher()
		{
			static Publisher* instance(new Publisher);
			return *instance;
		}
	};
};