#ifndef CANFD_ERROR_FRAME64_H
#define CANFD_ERROR_FRAME64_H

#include "../common.h"
#include "object_header.h"
#include "canfd_ext_frame_data.h"

BLFSTART
class TWBLF_API CanFdErrorFrame64 final : public ObjectHeader
{
public:
    uint8_t channel {};
    uint8_t dlc {};
    uint8_t valid_data_bytes {};            /* Valid payload length of mData */
    uint8_t ecc {};
    uint16_t flags {};
    uint16_t error_code_ext {};
    uint16_t ext_flags {};                  /* FD specific flags */
    uint8_t ext_data_offset {};             /* have ext_frame_data ? */
    uint8_t reserved1 {};
    uint32_t id {};
    uint32_t frame_length {};               /* message length in ns - without 3 inter frame space bits */

    uint32_t btr_cfg_arb {};                /* bit rate used in arbitration phase */
    uint32_t btr_cfg_data {};               /* bit rate used in data phase */
    uint32_t time_offset_brs_ns {};         /* time offset of brs field */
    uint32_t time_offset_crc_del_ns {};     /* time offset of crc delimiter field */
    uint32_t crc {};
    uint16_t error_position {};             /* error position as bit offset */
    uint16_t reserved2 {};
    std::array<uint8_t, 64> data {};           /* CAN FD data */
    CanFdExtFrameData ext_frame_data {};

    [[nodiscard]] uint32_t canfd_error_frame64_object_size() const
    {
        uint32_t size = object_header_object_size() +
                        sizeof(channel) +
                        sizeof(dlc) +
                        sizeof(valid_data_bytes) +
                        sizeof(ecc) +
                        sizeof(flags) +
                        sizeof(error_code_ext) +
                        sizeof(ext_flags) +
                        sizeof(ext_data_offset) +
                        sizeof(reserved1) +
                        sizeof(id) +
                        sizeof(frame_length) +
                        sizeof(btr_cfg_arb) +
                        sizeof(btr_cfg_data) +
                        sizeof(time_offset_brs_ns) +
                        sizeof(time_offset_crc_del_ns) +
                        sizeof(crc) +
                        sizeof(error_position) +
                        sizeof(reserved2) +
                        BL_DLC_TO_FRAME_LENGTH(this->dlc);

        if (ext_data_offset) {
            size += ext_frame_data.canfd_ext_frame_data_object_size();
        }
        return size;
    }
};
BLFEND
#endif //CANFD_ERROR_FRAME64_H
