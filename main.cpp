#include <iostream>
#include "src/include/logger.h"
#include "src/include/types.h"

int main()
{
	BLF::Logger::create(BLF::FileFormat::BLF);

	return 0;
}