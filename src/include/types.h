#ifndef BLF_TYPES_H
#define BLF_TYPES_H

#include <cstdint>

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

enum class FileFormat {
	BLF,
	ASC,
	TXT
};

enum class BusType {
	UNKNOWN = 0,
	CAN,
	CAN_FD,
	LIN,
	FLEXRAY,
	ETHERNET
};

class BusMessage {
public:
	virtual ~BusMessage() = default;

	[[nodiscard]] virtual BusType bus_type() const = 0;

	[[nodiscard]] virtual uint64_t timestamp() const = 0;

	virtual void set_timestamp(uint64_t timestamp_ns) = 0;

	[[nodiscard]] virtual uint16_t channel() const = 0;
};

} // namespace blf

#endif // BLF_TYPES_H
