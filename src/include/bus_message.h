#ifndef BLF_TYPES_H
#define BLF_TYPES_H

#include <cstdint>
#include "frame_type_define.h"

#ifdef _WIN32
	#ifdef BLF_EXPORTS
		#define BLF_API __declspec(dllexport)
	#else
		#define BLF_API __declspec(dllimport)
	#endif
#else
	#define BLF_API __attribute__((visibility("default")))
#endif

namespace BLF {

class BLF_API BusMessage
{
public:
	virtual ~BusMessage() = default;

	[[nodiscard]] virtual BusType get_bus_type() const = 0;

	[[nodiscard]] virtual uint64_t get_timestamp() const = 0;

	virtual void set_timestamp(uint64_t timestamp) = 0;
};

} // namespace blf

#endif // BLF_TYPES_H
