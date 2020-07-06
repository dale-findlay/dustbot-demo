#pragma once

#include "core\ExportMacro.h"

#include <string>

namespace Horizon
{
	class HORIZON_API JsonValue
	{
	public:
		JsonValue();
		JsonValue(const char* str);
		JsonValue(int i);
		JsonValue(bool b);
		JsonValue(float f);
		
		std::string GetString() const;
		int GetInt() const;
		bool GetBool() const;
		float GetFloat() const;

	private:
		std::string m_stringValue;
		int m_intValue;
		bool m_boolValue;
		float m_floatValue;
	};
}
