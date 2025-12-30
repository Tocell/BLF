#ifndef BLF_LOG_H
#define BLF_LOG_H

#ifdef _WIN32
	#ifdef CANCONTROL_EXPORTS
		#define CANCONTROL_API __declspec(dllexport)
	#else
		#define CANCONTROL_API __declspec(dllimport)
	#endif
#else
	#define CANCONTROL_API __attribute__((visibility("default")))
#endif
#include <cstdint>

CANCONTROL_API void openFile(const char* fileName);

CANCONTROL_API void closeFile();

CANCONTROL_API int32_t write();

CANCONTROL_API int32_t read();

#endif //BLF_LOG_H
