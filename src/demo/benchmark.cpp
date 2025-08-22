#include "can_message.h"
#include "log_container.h"
#include "file_statistics.h"
#include "file.h"
#include "canfd_message.h"
#include "canfd_message64.h"
#include "flexray_vfr_receive_msg.h"
#include "flexray_vfr_receive_msg_ex.h"
#include "lin_message.h"

#include <benchmark/benchmark.h>
#include <chrono>
#include <string>

#define OBJECT_SIZE 1000000

uint64_t posix_time_us_uint64()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

static void BM_BLFWrite_CorrectLifecycle(benchmark::State& state) {
    const int64_t bytes_per_loop = OBJECT_SIZE * (
        sizeof(BLF::CanMessage) +
        sizeof(BLF::CanFdMessage) +
        sizeof(BLF::CanFdMessage64) +
        sizeof(BLF::FlexrayVFrReceiveMsgEx) +
        sizeof(BLF::LinMessage)
    );

    for (auto _ : state) {
        BLF::File file;
        file.open("benchmark_write_final.blf", std::ios::out | std::ios::trunc);
        file.file_statistics_.compression_level = 2;

        for (int i = 0; i < OBJECT_SIZE; i++) {
            auto* can_message = new BLF::CanMessage();
            can_message->object_timestamp = posix_time_us_uint64();
            can_message->signature = BL_OBJ_SIGNATURE;
            can_message->header_version = 1;
            can_message->object_type = BL_OBJ_TYPE_CAN_MESSAGE;
            can_message->object_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
            can_message->client_index = 1;
            can_message->object_version = 0;
            can_message->object_timestamp = posix_time_us_uint64();
            can_message->channel = 1;
            can_message->id = 10;
            can_message->dlc = 8;
            can_message->flags |= CAN_MESSAGE_FLAG_TX;
            for (int j = 0 ; j < can_message->dlc; j++)
                can_message->data[0] = 10;
            file.write(can_message);

            auto* canfd_message = new BLF::CanFdMessage();
            canfd_message->object_timestamp = posix_time_us_uint64();
            canfd_message->signature = BL_OBJ_SIGNATURE;
            canfd_message->header_version = 1;
            canfd_message->object_type = BL_OBJ_TYPE_CAN_FD_MESSAGE;
            canfd_message->object_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
            canfd_message->client_index = 1;
            canfd_message->object_version = 0;
            canfd_message->object_timestamp = posix_time_us_uint64();

            canfd_message->channel = 2;
            canfd_message->flags |= CANFD_MESSAGE_FLAG_TX;
            canfd_message->dlc = 15;
            canfd_message->id = 100 ;//| 0x80000000;
            canfd_message->canfd_flags = CANFD_MESSAGE_CANFD_FLAGS_BRS | CANFD_MESSAGE_CANFD_FLAGS_EDL;
            canfd_message->valid_data_bytes = 64;
            for (int j = 0; j < 64; j++)
                canfd_message->data[j] = j + 1;
            file.write(canfd_message);

            auto* canfd_message64 = new BLF::CanFdMessage64();
            canfd_message64->object_timestamp = posix_time_us_uint64();
            canfd_message64->header_version = 1;
            canfd_message64->object_type = BL_OBJ_TYPE_CAN_FD_MESSAGE_64;
            canfd_message64->object_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
            canfd_message64->object_version = 0;
            canfd_message64->object_timestamp = posix_time_us_uint64();

            canfd_message64->flags = CANFD_MESSAGE64_FLAGS_BRS | CANFD_MESSAGE64_FLAGS_EDL;
            canfd_message64->ext_data_offset = 0;
            canfd_message64->channel = 1;
            canfd_message64->dlc = 15;
            canfd_message64->id = 200 ;//| 0x80000000;
            canfd_message64->valid_data_bytes = 64;
            for (int j = 0; j < canfd_message64->valid_data_bytes; j++)
                canfd_message64->data[j] = j + 1;
            file.write(canfd_message64);

            auto* flexray_msg = new BLF::FlexrayVFrReceiveMsgEx();
            flexray_msg->object_timestamp = posix_time_us_uint64();
            flexray_msg->header_version = 1;
            flexray_msg->object_type = BL_OBJ_TYPE_FR_RCVMESSAGE_EX;
            flexray_msg->object_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
            flexray_msg->object_version = 0;
            flexray_msg->object_timestamp = posix_time_us_uint64();

            flexray_msg->channel = 1;
            flexray_msg->channel_mask = 0x01;
            flexray_msg->frame_id = 300;
            flexray_msg->byte_count = 520;
            flexray_msg->data_count = 32;
            flexray_msg->cycle = 16;
            flexray_msg->tag = 5;
            flexray_msg->frame_flags = 0x2;
            flexray_msg->header_crc1 = 0x3;
            flexray_msg->header_crc2 = 0x0;
            for (int k = 0; k < flexray_msg->data_count; k++)
            {
                flexray_msg->data_bytes[k] = k + 1;
            }
            flexray_msg->version = 0;
            flexray_msg->dir = 0;               // Tx
            flexray_msg->client_index = 0;
            flexray_msg->cluster_no = 0;
            flexray_msg->data = 0x0;
            flexray_msg->app_parameter = 0;
            flexray_msg->frame_length_ns = 0;
            flexray_msg->frame_id1 = 0;
            flexray_msg->pdu_offset = 0;
            flexray_msg->blf_log_mask = 0;
            file.write(flexray_msg);

            auto* lin_msg = new BLF::LinMessage();
            lin_msg->object_timestamp = posix_time_us_uint64();
            lin_msg->object_type = BL_OBJ_TYPE_LIN_MESSAGE;
            lin_msg->object_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
            lin_msg->client_index = 1;
            lin_msg->object_version = 0;
            lin_msg->object_timestamp = posix_time_us_uint64();

            lin_msg->channel = 1;
            lin_msg->dlc = 8;
            lin_msg->id = 22 ;
            for (int j = 0; j < lin_msg->dlc; j++)
                lin_msg->data[j] = j + 1;
            lin_msg->dir = 0;
            lin_msg->fsm_id = 0x01;
            lin_msg->fsm_state = 0x02;
            lin_msg->header_time = 100;
            lin_msg->full_time = 105;
            lin_msg->reserved = 0;
            file.write(lin_msg);
        }
        file.close();

        // 报告这次迭代处理的字节数，用于计算速率
        state.SetBytesProcessed(bytes_per_loop);
    }
}
BENCHMARK(BM_BLFWrite_CorrectLifecycle);

BENCHMARK_MAIN();