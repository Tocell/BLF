#include "logger.h"
#include "blf_logger.h"
#include "asc_logger.h"

#include <iostream>

namespace BLF
{

std::unique_ptr<Logger> Logger::create(FileFormat format)
{
	switch (format)
	{
	case FileFormat::BLF:
		std::cout << "Create Logger Blf" << std::endl;
		return std::make_unique<BlfLogger>();
	case FileFormat::ASC:
		// return std::make_unique<AscLogger>();
	case FileFormat::TXT:
		// return std::make_unique<TxtLogger>(); // 如果有的话
	default:
		// 或者抛出异常
		return nullptr;
	}
}


}