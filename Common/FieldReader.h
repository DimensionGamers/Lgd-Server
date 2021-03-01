#ifndef FIELD_READER_H
#define FIELD_READER_H

#pragma once

class FieldReader
{
public:
	FieldReader(Field* fields) : _fields(fields), read_index(0) { }

	bool GetBool() { return _fields[read_index++].GetBool(); }
	uint8 GetUInt8() { return _fields[read_index++].GetUInt8(); }
	int8 GetInt8() { return _fields[read_index++].GetInt8(); }
	uint16 GetUInt16() { return _fields[read_index++].GetUInt16(); }
	int16 GetInt16() { return _fields[read_index++].GetInt16(); }
	uint32 GetUInt32() { return _fields[read_index++].GetUInt32(); }
	int32 GetInt32() { return _fields[read_index++].GetInt32(); }
	uint64 GetUInt64() { return _fields[read_index++].GetUInt64(); }
	int64 GetInt64() { return _fields[read_index++].GetInt64(); }
	float GetFloat() { return _fields[read_index++].GetFloat(); }
	double GetDouble() { return _fields[read_index++].GetDouble(); }
	char const* GetCString() { return _fields[read_index++].GetCString(); }
	std::string GetString() { return _fields[read_index++].GetString(); }
	std::string GetBase64String() { return _fields[read_index++].GetBase64String(); }
	//std::vector<uint8> GetBinary() { return _fields[read_index++].GetBinary(); }
	bool IsNull() { return _fields[read_index++].IsNull(); }
	void Skip() { Skip(1); }
	void Skip(int32 amount) { read_index += amount; }

	uint16 ReadItem()
	{
		uint8 type = GetUInt8();
		uint16 index = GetUInt16();

		return ITEMGET(type, index);
	}
private:
	Field* _fields;
	int32 read_index;
};

#endif