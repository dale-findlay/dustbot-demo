#include "JsonValue.h"

Horizon::JsonValue::JsonValue()
{
}

Horizon::JsonValue::JsonValue(const char* str)
	:m_stringValue(str)
{
}

Horizon::JsonValue::JsonValue(int i)
	: m_intValue(i)
{
}

Horizon::JsonValue::JsonValue(bool b)
	: m_boolValue(b)
{}

Horizon::JsonValue::JsonValue(float f)
	: m_floatValue(f)
{}

std::string Horizon::JsonValue::GetString() const
{
	return m_stringValue;
}

int Horizon::JsonValue::GetInt() const
{
	return m_intValue;
}

bool Horizon::JsonValue::GetBool() const
{
	return m_boolValue;
}

float Horizon::JsonValue::GetFloat() const
{
	return m_floatValue;
}
