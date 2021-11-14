#pragma once

namespace DZB
{
	class Renderer
	{
		virtual void Begin() = 0;
		
		virtual void End() = 0;
		virtual void Flush() = 0;
	public:
		int drawCalls;
	};
}