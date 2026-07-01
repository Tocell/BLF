#ifndef ETHERNET_MESSAGE_H
#define ETHERNET_MESSAGE_H

#include "ethernet_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API EthernetMessage : public BusMessage
{
public:
	explicit EthernetMessage(const EthernetFrame& frame);
	~EthernetMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;
	[[nodiscard]] uint64_t get_timestamp() const override;
	void set_timestamp(uint64_t timestamp) override;
	[[nodiscard]] const EthernetFrame& get_frame() const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<EthernetFrame> { using type = EthernetMessage; };

}

#endif //ETHERNET_MESSAGE_H
