#ifndef BLF_OBJECTS_H
#define BLF_OBJECTS_H

#include <cstdint>
#pragma pack(push, 1)
struct ObjectHeaderBase
{
	uint32_t signature;
	uint16_t header_size;
	uint16_t header_version;
	uint32_t object_size;
	uint32_t object_type;
};

struct ObjectHeader
{
	ObjectHeaderBase base;
	uint32_t object_flags;
	uint16_t client_index;
	uint16_t object_version;
	uint64_t object_timestamp;
};

struct ObjectHeader2
{
	ObjectHeaderBase base;
	uint32_t object_flags;
	uint8_t timestamp_status;
	uint16_t client_index;
	uint16_t object_version;
	uint64_t object_timestamp;
};

struct SYSTEMTIME
{
	uint16_t year;
	uint16_t month;
	uint16_t dayOf_week;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint16_t milliseconds;
};



struct CanMessage
{
	ObjectHeaderBase header_base;
	ObjectHeader header;
	uint16_t channel;
	uint8_t flags;
	uint8_t dlc;
	uint32_t id;
	uint8_t data[8];
};
#pragma pack(pop)

#endif //BLF_OBJECTS_H
