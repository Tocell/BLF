#ifndef CANFD_MESSAGE_ASC_READER_H
#define CANFD_MESSAGE_ASC_READER_H
#include "iasc_message_reader.h"
#include "asc_reader_registrar.h"
#include "frame_type_define.h"
#include "file_reader.h"

namespace GWLogger::Asc
{

class CanFdMessageAscReader : public IAscMessageReader
{
public:
	~CanFdMessageAscReader() override = default;

	// static constexpr uint32_t kKey = BL_OBJ_TYPE_CAN_FD_MESSAGE;

	[[nodiscard]] BusMessagePtr read_line(const std::string& line,
							uint64_t file_start_posix_us) const override;

	[[nodiscard]] bool match(const std::string& line) const override;
};

}

#endif //CANFD_MESSAGE_ASC_READER_H
