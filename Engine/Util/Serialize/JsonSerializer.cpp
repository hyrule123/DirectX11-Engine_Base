
#include "JsonSerializer.h"

#include <Engine/Common.h>

namespace core
{
	void operator<<(Json::Value& _jVal, const char* _c_str)
	{
		if (nullptr != _c_str)
		{
			_jVal = Json::StaticString(_c_str);
		}
	}
	void operator>>(const Json::Value& _jVal, const char* _c_str)
	{
		const auto& pair = g_string_archive.insert(_jVal.asString());
		_c_str = pair.first->c_str();
	}

	void operator<<(Json::Value& _jVal, const float3& _f3)
	{
		_jVal.clear();
		_jVal.append(_f3.x);
		_jVal.append(_f3.y);
		_jVal.append(_f3.z);
	}

	void operator>>(const Json::Value& _jVal, float3& _f3)
	{
		_f3.x = _jVal[0].asFloat();
		_f3.y = _jVal[1].asFloat();
		_f3.z = _jVal[2].asFloat();
	}

	void operator<<(Json::Value& _jVal, const float4& _f4)
	{
		_jVal.clear();
		_jVal.append(_f4.x);
		_jVal.append(_f4.y);
		_jVal.append(_f4.z);
		_jVal.append(_f4.w);
	}

	void operator>>(const Json::Value& _jVal, float4& _f4)
	{
		_f4.x = _jVal[0].asFloat();
		_f4.y = _jVal[1].asFloat();
		_f4.z = _jVal[2].asFloat();
		_f4.w = _jVal[3].asFloat();
	}

	void operator<<(Json::Value& _jVal, const MATRIX& _m)
	{
		_jVal.clear();
		_jVal.resize(4);
		for (int i = 0; i < 4; ++i)
		{
			Json::Value& col = _jVal[i];
			col.resize(4);
			for (int j = 0; j < 4; ++j)
			{
				col[j] = _m.m[i][j];
			}
		}
	}
	void operator>>(const Json::Value& _jVal, MATRIX& _m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				_m.m[i][j] = _jVal[i][j].asFloat();
			}
		}
	}
}



