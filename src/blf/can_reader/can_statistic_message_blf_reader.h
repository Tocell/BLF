#ifndef CAN_STATISTIC_MESSAGE_BLF_READER_H
#define CAN_STATISTIC_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanStatisticMessageBlfReader : public IMessageReader
{
public:
	CanStatisticMessageBlfReader();
	~CanStatisticMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_STATISTIC_MESSAGE_BLF_READER_H
