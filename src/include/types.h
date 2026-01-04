#ifndef BLF_TYPES_H
#define BLF_TYPES_H

#include <cstdint>
#include <memory>

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


class BLF_API BusMessage
{
public:
	virtual ~BusMessage() = default;

	[[nodiscard]] virtual BusType get_bus_type() const = 0;

	// [[nodiscard]] virtual uint64_t get_timestamp() const = 0;
	//
	// virtual void set_timestamp(uint64_t timestamp_ns) = 0;
	//
	// virtual void set_channel(uint16_t channel) = 0;
	//
	// [[nodiscard]] virtual uint16_t get_channel() const = 0;
};

} // namespace blf

#endif // BLF_TYPES_H
