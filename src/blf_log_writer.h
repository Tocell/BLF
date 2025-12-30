#ifndef BLF_WRITER_H
#define BLF_WRITER_H
#include "blf_log_file.h"

class BlfLogWriter : public BlfLogFile
{
public:
	BlfLogWriter() = default;

	~BlfLogWriter() override = default;

	int32_t write() override;
};

#endif //BLF_WRITER_H
