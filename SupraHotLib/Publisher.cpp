#include "Publisher.h"


namespace SupraHot
{
	namespace PubSub
	{
		Publisher::Publisher()
		{
			Subscribers = new std::map<std::string, std::vector<const Subscriber*>*>;
		}

		Publisher::~Publisher()
		{
		}

		void Publisher::Publish(const std::string& message, void* data)
		{
			if (this->Subscribers->at(message) != nullptr)
			{
				std::vector<const Subscriber*>* subscriberVector = this->Subscribers->at(message);

				for (uint32 i = 0, l = static_cast<uint32>(subscriberVector->size()); i < l; ++i)
				{
					Subscriber* sub = const_cast<Subscriber*>(subscriberVector->at(i));
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
			if (Subscribers->find(message) == Subscribers->end())
			{
				(*Subscribers)[message] = new std::vector<const Subscriber*>;
			}

			std::vector<const Subscriber*>* subscriberVector = this->Subscribers->at(message);

			for (uint32 i = 0, l = static_cast<uint32>(subscriberVector->size()); i < l; ++i)
			{
				const Subscriber* sub = subscriberVector->at(i);
				if (sub == subscriber)
				{
					// Subscriber already get notyfied for this message
					return;
				}
			}

			subscriberVector->push_back(subscriber);
		}

		void Publisher::Delete()
		{
			typedef std::map<std::string, std::vector<const Subscriber*>*>::iterator it_type;
			for (it_type iterator = this->Subscribers->begin(); iterator != this->Subscribers->end(); ++iterator) {
				// Delete all subscriber vectors
				delete iterator->second;
			}
		}
	};
};