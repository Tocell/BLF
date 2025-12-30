#ifndef ASC_WRITER_H
#define ASC_WRITER_H
#include "asc_log_file.h"

class AscLogWriter : public AscLogFile
{
public:
	AscLogWriter() = default;
	~AscLogWriter() override = default;

	int32_t write();
};

#endif //ASC_WRITER_H
