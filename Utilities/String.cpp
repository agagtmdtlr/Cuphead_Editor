#include "stdafx.h"
#include "String.h"

//////////////////////////////////////////////////////////////////////////
///@brief 문자열 자르기
///@param orgin : 원본 문자열
///@param tok : 자를 기준이 되는 문자열
///@return 완료된 문자열 배열
//////////////////////////////////////////////////////////////////////////
void String::SplitString(vector<string>* result, string origin, string tok) // 문자열 특정문자를 기준으로 분리하기
{
	// 아스키 코드
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}

void String::SplitString(vector<wstring>* result, wstring origin, wstring tok)
{
	// 유니코드
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}


//////////////////////////////////////////////////////////////////////////
///@brief 시작 문자열이 같은지 체크
///@param str : 체크하려는 문자열
///@param comp : 시작 비교문자열
//////////////////////////////////////////////////////////////////////////
bool String::StartsWith(string str, string comp)
{
	//아스키코드
	wstring::size_type index = str.find(comp); // 비교 문자열이 체크 문자열에서 시작되는 인덱스 반환
	if (index != wstring::npos && (int)index == 0) // 해당 문자열이 포함되어있고 인덱스가 0일 경우
		return true;

	return false;
}

bool String::StartsWith(wstring str, wstring comp)
{
	// 유니코드
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && (int)index == 0)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
///@brief 해당 문자열이 포함되어 있는지
///@param str : 체크하려는 문자열
///@param comp : 비교문자열
//////////////////////////////////////////////////////////////////////////
bool String::Contain(string str, string comp)
{
	size_t found = str.find(comp);

	return found != wstring::npos;
}

bool String::Contain(wstring str, wstring comp)
{
	size_t found = str.find(comp);

	return found != wstring::npos;
}


//////////////////////////////////////////////////////////////////////////
///@brief 해당 문자열에서 comp 문자를 rep로 변경
///@param str : 체크하려는 문자열
///@param comp : 비교문자열
///@param rep : 바꿀문자열
//////////////////////////////////////////////////////////////////////////
void String::Replace(string * str, string comp, string rep)
{
	string temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	*str = temp;
}

void String::Replace(wstring* str, wstring comp, wstring rep)
{
	wstring temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		// 비교 문자열 부분을 치환 문자열로 대체 한다.
		temp.replace(start_pos, comp.length(), rep);
		// 치환된 문자열 부분의 끝 인덱스 부터 다시 탐색을 시작한다.
		start_pos += rep.length();
	}

	// 작업이 완료된 문자열을 갱신한다.
	*str = temp;
}

//////////////////////////////////////////////////////////////////////////
///@brief string형을 wstring형으로 변경
///@param value : 변환할 문자열
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
wstring String::ToWString(string value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());

	return temp;
}

//////////////////////////////////////////////////////////////////////////
///@brief wstring형을 string형으로 변경
///@param value : 변환할 문자열
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
string String::ToString(wstring value)
{
	string temp = "";
	temp.assign(value.begin(), value.end());

	return temp;
}
//////////////////////////////////////////////////////////////////////////