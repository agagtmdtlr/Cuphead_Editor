#include "stdafx.h"
#include "Time.h"

Time::Time()
	:elapsed(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&tick); // 1�ʴ� Ŭ����
	QueryPerformanceCounter((LARGE_INTEGER *)&start); // ���� Ŭ����

	last = start;
}

Time::~Time()
{

}

void Time::Update()
{
	__int64 current;
	QueryPerformanceCounter((LARGE_INTEGER *)&current); // ���� Ŭ����

	elapsed = current - last; // ����� ������
	last = current; // ���������� ������Ʈ�� Ŭ����
}

float Time::Running() // Ÿ�̸� �������� ����� �ð�
{
	__int64 current;
	QueryPerformanceCounter((LARGE_INTEGER *)&current);

	return (float)((double)(current - start) / tick);
}

float Time::Elapsed() // ������ ���� ����� �ð�
{
	return (float)elapsed / tick;
}