#include "stdafx.h"
#include "Time.h"

Time::Time()
	:elapsed(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&tick); // 1초당 클럭수
	QueryPerformanceCounter((LARGE_INTEGER *)&start); // 시작 클럭수

	last = start;
}

Time::~Time()
{

}

void Time::Update()
{
	__int64 current;
	QueryPerformanceCounter((LARGE_INTEGER *)&current); // 현재 클럭수

	elapsed = current - last; // 경과된 진동수
	last = current; // 마지막으로 업데이트된 클럭수
}

float Time::Running() // 타이머 시작으로 경과된 시간
{
	__int64 current;
	QueryPerformanceCounter((LARGE_INTEGER *)&current);

	return (float)((double)(current - start) / tick);
}

float Time::Elapsed() // 프레임 마다 경과된 시간
{
	return (float)elapsed / tick;
}