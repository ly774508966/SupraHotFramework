#include "RenderCommandQueue.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		RenderCommandQueue::RenderCommandQueue()
		{
	
		}

		RenderCommandQueue::~RenderCommandQueue()
		{
		}

	/*	RenderCommandQueue& RenderCommandQueue::GetInstance()
		{
			static RenderCommandQueue* instance(new RenderCommandQueue);
			return *instance;
		}*/

		void RenderCommandQueue::AddCommand(RenderCommand* renderCommand)
		{
			if (std::find(RenderCommands.begin(), RenderCommands.end(), renderCommand) == RenderCommands.end())
			{
				RenderCommands.push_back(renderCommand);
			}
		}

		void RenderCommandQueue::Execute()
		{
			for (size_t i = 0, l = RenderCommands.size(); i < l; ++i)
			{
				RenderCommands[i]->Execute(&State);
			}
		}

		void RenderCommandQueue::Clear()
		{
			for (size_t i = 0, l = RenderCommands.size(); i < l; ++i)
			{
				delete RenderCommands[i];
			}
			RenderCommands.clear();
		}

		size_t RenderCommandQueue::Size()
		{
			return RenderCommands.size();
		}
	};
};