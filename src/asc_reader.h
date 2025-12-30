#ifndef ASC_READER_H
#define ASC_READER_H
#include "log_reader.h"

class AscReader : public LogReader
{
public:
	AscReader() = default;
	~AscReader() override = default;

	int32_t read() override;
};

#endif //ASC_READER_H
