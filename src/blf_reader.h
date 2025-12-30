#ifndef BLF_READER_H
#define BLF_READER_H

#include "blf_log_file.h"

class BlfLogReader : public BlfLogFile
{
public:
	BlfLogReader() = default;
	~BlfLogReader() override = default;

	int32_t read();
};

#endif //BLF_READER_H
