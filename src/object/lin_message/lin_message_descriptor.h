#ifndef LIN_MESSAGE_DESCRIPTOR_H
#define LIN_MESSAGE_DESCRIPTOR_H

#include "../common.h"
#include "object_header.h"
#include "lin_synch_field_event.h"
#include <array>


BLFSTART
    class TWBLF_API LinMessageDescriptor : public LinSynchFieldEvent
{
public:
    uint16_t supplierId {};
    uint16_t messageId {};
    uint8_t nad {};
    uint8_t id {};
    uint8_t dlc {};
    uint8_t checksum_model {};

    [[nodiscard]] uint32_t lin_message_descriptor_object_size() const
    {
        return lin_synch_field_event_object_size() +
               sizeof(supplierId) +
               sizeof(messageId) +
               sizeof(nad) +
               sizeof(id) +
               sizeof(dlc) +
               sizeof(checksum_model);
    }
};
BLFEND

#endif //LIN_MESSAGE_DESCRIPTOR_H
