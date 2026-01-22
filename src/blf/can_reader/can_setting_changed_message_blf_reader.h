#ifndef CAN_SETTING_CHANGED_MESSAGE_BLF_READER_H
#define CAN_SETTING_CHANGED_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanSettingChangedMessageBlfReader : public IMessageReader
{
public:
	CanSettingChangedMessageBlfReader();
	~CanSettingChangedMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_SETTING_CHANGED_MESSAGE_BLF_READER_H
