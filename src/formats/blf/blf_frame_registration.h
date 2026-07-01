#ifndef BLF_FRAME_REGISTRATION_H
#define BLF_FRAME_REGISTRATION_H

#include "blf_frame_codec.h"
#include "blf_reader_registrar.h"
#include "writer_registrar.h"

#define GWLOGGER_REGISTER_BLF_FRAME_WRITER(WriterName, MessageType, FrameType, ObjectType, BusTypeValue) \
	namespace { \
	using WriterName = ::GWLogger::Blf::BlfFrameWriter<MessageType, FrameType, ObjectType>; \
	static ::GWLogger::WriterRegistrar<WriterName> WriterName##_registrar(::GWLogger::FileFormat::BLF, BusTypeValue); \
	}

#define GWLOGGER_REGISTER_BLF_WRITER(WriterType, BusTypeValue) \
	namespace { \
	static ::GWLogger::WriterRegistrar<WriterType> WriterType##_registrar(::GWLogger::FileFormat::BLF, BusTypeValue); \
	}

#define GWLOGGER_REGISTER_BLF_FRAME_READER(ReaderName, FrameType, ObjectType) \
	namespace { \
	using ReaderName = ::GWLogger::Blf::BlfFrameReader<FrameType, ObjectType>; \
	static ::GWLogger::Blf::ReaderRegistrar<ReaderName> ReaderName##_registrar(ObjectType); \
	}

#define GWLOGGER_REGISTER_BLF_READER(ReaderType, ObjectType) \
	namespace { \
	static ::GWLogger::Blf::ReaderRegistrar<ReaderType> ReaderType##_registrar(ObjectType); \
	}

#endif // BLF_FRAME_REGISTRATION_H
