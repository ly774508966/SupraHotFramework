#include "Subscriber.h"
#include "Publisher.h"

namespace SupraHot
{
	namespace PubSub
	{
		Subscriber::Subscriber()
		{
		}

		Subscriber::~Subscriber()
		{
		}

		void Subscriber::Subscribe(Publisher& publisher, std::string message)
		{
			publisher.AddSubscriber(this, message);
		}

		void Subscriber::Unsubscribe(Publisher& publisher, std::string message)
		{
			publisher.RemoveSubscriber(this, message);
		}
	};
};