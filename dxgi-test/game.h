#ifndef DXGI_GAME_H_
#define DXGI_GAME_H_

#include "unordered_map"
#include "duplication_manager.h"
#include "logger.h"

namespace game {
	POINT BeginPoint(const RECT& rect);
	bool IsInRect(const RECT& rect, const POINT& p);
	void Next(const RECT& rect, POINT* p);
	void SaveFrameAsPng(dupl::FrameData* frame, const unsigned hash);

	class Filter
	{
	public:
		virtual bool Run(dupl::FrameData* frame_data) = 0;
	};

	class Controller
	{
	public:
		Controller();
		~Controller();
		ReturnStatus Init();
		void AddFilter(Filter* filter);
		ReturnStatus StartGaming();

	private:
		dupl::DuplicationManager* dupl_manager_;
		std::vector<Filter*> filters_;
	};

}

#endif // DXGI_GAME_H_
