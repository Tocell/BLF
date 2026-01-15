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
	CAN2,
	CAN_FD,
	LIN,
	FLEXRAY,
	ETHERNET
};

enum class OpenMode {
	Read,
	Write
};

#define TIME_TEN_MICS     0x00000001 /* 10 micro second timestamp */
#define TIME_ONE_NANS     0x00000002 /* 1 nano second timestamp */


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
