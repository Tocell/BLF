#ifndef LOG_CONTAINER_H
#define LOG_CONTAINER_H

#include "blf_structure.h"

namespace BLF
{

class LogContainerHandler
{
public:
	LogContainerHandler();
	~LogContainerHandler() = default;

	void compress(uint16_t compression_method, int compression_level);

	void uncompress();

	LogContainer& get_logcontainer();

	void set_buffer(uint8_t* data, size_t size);

	void reset_logcontainer();

	size_t calculate_size();

private:
	LogContainer log_container_{};
};

}

#endif //LOG_CONTAINER_H
