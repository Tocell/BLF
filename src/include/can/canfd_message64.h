#ifndef CANFD_MESSAGE64_H
#define CANFD_MESSAGE64_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanFdMessage64 : public BusMessage
{
public:
	explicit CanFdMessage64(const CanFdFrame64& frame64);
	~CanFdMessage64() override;


	[[nodiscard]] BusType get_bus_type() const override;

	[[nodiscard]] uint64_t get_timestamp() const override;

	void set_timestamp(uint64_t timestamp) override;

	[[nodiscard]] const CanFdFrame64& get_frame() const;
private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<CanFdFrame64> { using type = CanFdMessage64; };

}
#endif //CANFD_MESSAGE64_H
