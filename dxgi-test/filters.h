#ifndef GAME_FILTERS_H_
#define GAME_FILTERS_H_

#include "game.h"
namespace game
{
	class WeaponFilter : public Filter
	{
	public:
		bool Run(dupl::FrameData* frame_data, Context* context);
	};

	class IsInRangeFilter : public Filter
	{
	public:
		bool Run(dupl::FrameData* frame_data, Context* context);
	};

	class SniperFilter : public Filter
	{
	public:
		bool Run(dupl::FrameData* frame_data, Context* context);
	private:
		enum State
		{
			NON_SNIPE,
			SNIPE,
			SLEEPING
		} state_;
		DWORD weekup_tick;
		bool NonSnipeRun(dupl::FrameData* frame_data, Context* context);
		bool SnipeRun(dupl::FrameData* frame_data, Context* context);
		bool SleepRun(dupl::FrameData* frame_data, Context* context);
	};
}

#endif