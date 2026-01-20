#ifndef BLF_TYPES_H
#define BLF_TYPES_H

#include <cstdint>
#include "frame_type_define.h"

#ifdef _WIN32
	#ifdef GWLOGGER_EXPORTS
		#define GWLOGGER_API __declspec(dllexport)
	#else
		#define GWLOGGER_API __declspec(dllimport)
	#endif
#else
	#define GWLOGGER_API __attribute__((visibility("default")))
#endif

namespace GWLogger {

class GWLOGGER_API BusMessage
{
public:
	virtual ~BusMessage() = default;

	[[nodiscard]] virtual BusType get_bus_type() const = 0;

	[[nodiscard]] virtual uint64_t get_timestamp() const = 0;

	virtual void set_timestamp(uint64_t timestamp) = 0;
};

} // namespace blf

#endif // BLF_TYPES_H
