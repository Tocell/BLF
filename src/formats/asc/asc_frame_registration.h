#ifndef ASC_FRAME_REGISTRATION_H
#define ASC_FRAME_REGISTRATION_H

#include "writer_registrar.h"
#include "asc_reader_registrar.h"

#define GWLOGGER_REGISTER_ASC_FRAME_WRITER(WriterType, BusTypeValue) \
	namespace { \
	static ::GWLogger::WriterRegistrar<WriterType> WriterType##_registrar(::GWLogger::FileFormat::ASC, BusTypeValue); \
	}

#define GWLOGGER_REGISTER_ASC_FRAME_READER(ReaderType, LineKeyValue) \
	namespace { \
	static ::GWLogger::Asc::AscReaderRegistrar<ReaderType> ReaderType##_registrar(LineKeyValue); \
	}

#endif //ASC_FRAME_REGISTRATION_H
