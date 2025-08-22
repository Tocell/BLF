#ifndef LIN_SEND_ERROR2_H
#define LIN_SEND_ERROR2_H

#include "../common.h"
#include "object_header.h"
#include "lin_message_descriptor.h"

BLFSTART
class TWBLF_API LinSendError2 : public ObjectHeader
{
public:
    LinMessageDescriptor lin_msg_descr_event;
    uint64_t eoh {};
    uint8_t is_etf {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    uint8_t reserved1 {};
    std::array<uint8_t, 4> reserved2 {};
    double exact_header_baudrate {};
    uint32_t early_stopbit_offset {};

    [[nodiscard]] uint32_t lin_send_error2_object_size() const
    {
        return object_header_object_size() +
               lin_msg_descr_event.lin_message_descriptor_object_size() +
               sizeof(eoh) +
               sizeof(is_etf) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(reserved1) +
               sizeof(reserved2) +
               sizeof(exact_header_baudrate) +
               sizeof(early_stopbit_offset);
    }
};
BLFEND
#endif //LIN_SEND_ERROR2_H
