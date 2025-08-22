#include <iostream>
#include "../object/common.h"
#include "file.h"
#include <thread>
#include "can_message.h"
#include "log_container.h"
#include "file_statistics.h"
#include "file.h"
#include "canfd_message.h"
#include "canfd_message64.h"
#include "flexray_vfr_receive_msg.h"
#include "flexray_vfr_receive_msg_ex.h"
#include "lin_message.h"

int main(int argc, char* argv[])
{
    BLF::File file;
    file.open("example.blf");
    if (!file.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        return -1;
    }

    std::atomic<bool> is_running = true;
    std::thread tid = std::thread([&file, &is_running]()
    {
        while (is_running)
        {
            printf("file object queue size = %lld  object count = %lld \n",
                file.get_object_queue_size(), file.get_object_count());
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });

    BLF::FileStatistics file_statistics_ = file.read_file_statistics();


    int32_t object_count = 0;
    std::thread tid2 = std::thread([&is_running, &file, &object_count]()
    {
        while (is_running)
        {
            auto* object = file.read();
            if (!object) continue;
            if (object->object_type == BL_OBJ_TYPE_CAN_MESSAGE)
            {
                auto* obj = reinterpret_cast<BLF::CanMessage*>(object);
                printf("object count = %d ",object_count);
                printf("can object_timestamp = %lld", obj->object_timestamp);
                printf("can channel: %d ",obj->channel);
                printf("can id: %d ",obj->id);
                printf("can dlc = %d ", obj->dlc);
                printf("can flags = %d", obj->flags);
                printf("can data: ");
                for (int i = 0; i < obj->dlc; i++)
                {
                    printf("%x ",obj->data[i]);
                }
                printf("\n");
            }
            else if (object->object_type == BL_OBJ_TYPE_CAN_FD_MESSAGE)
            {
                auto* obj = reinterpret_cast<BLF::CanFdMessage*>(object);
                printf("object count = %d ",object_count);
                printf("canfd object_timestamp = %lld", obj->object_timestamp);
                printf("canfd channel: %d ",obj->channel);
                printf("canfd id: %d ",obj->id);
                printf("canfd dlc = %d ", obj->dlc);
                printf("canfd flags = %d", obj->flags);
                printf("canfd valid_data_bytes = %d", obj->valid_data_bytes);
                printf("canfd data: ");
                for (int i = 0; i < obj->valid_data_bytes; i++)
                {
                    printf("%x ",obj->data[i]);
                }
                printf("\n");
            }
            else if (object->object_type == BL_OBJ_TYPE_CAN_FD_MESSAGE_64)
            {
                auto* obj = reinterpret_cast<BLF::CanFdMessage64*>(object);
                printf("object count = %d ",object_count);
                printf("canfd64 object_timestamp = %lld ", obj->object_timestamp);
                printf("canfd64 channel: %d ",obj->channel);
                printf("canfd64 id: %d ",obj->id);
                printf("canfd64 dlc = %d ", obj->dlc);
                printf("canfd64 flags = %d ", obj->flags);
                printf("canfd64 valid_data_bytes = %d ", obj->valid_data_bytes);
                printf("canfd64 data: ");
                for (int i = 0; i < obj->valid_data_bytes; i++)
                {
                    printf("%02x ",obj->data[i]);
                }
                printf("\n");
            }
            else if (object->object_type == BL_OBJ_TYPE_FR_RCVMESSAGE_EX)
            {
                auto* obj = reinterpret_cast<BLF::FlexrayVFrReceiveMsgEx*>(object);
                printf("object count = %d ",object_count);
                printf("flexray  object_timestamp = %lld ", obj->object_timestamp);
                printf("flexray  channel = %d ", obj->channel);
                printf("flexray  channel_mask = %d ", obj->channel_mask);
                printf("flexray  frame_id: %d ",obj->frame_id);
                printf("flexray  byte_count = %d ", obj->byte_count);
                printf("flexray  data_count = %d ", obj->data_count);
                printf("flexray  cycle: %d ",obj->cycle);
                printf("flexray  tag = %d ", obj->tag);
                printf("flexray  frame_flags = %d ", obj->frame_flags);
                printf("flexray  header_crc1: %d ",obj->header_crc1);
                printf("flexray  header_crc2 = %d ", obj->header_crc2);
                printf("flexray  data: ");
                for (int i = 0; i < obj->data_count; i++)
                {
                    printf("%02x ", obj->data_bytes[i]);
                }
                printf("\n");
            }
            else if (object->object_type == BL_OBJ_TYPE_LIN_MESSAGE)
            {
                auto* obj = reinterpret_cast<BLF::LinMessage*>(object);
                printf("lin_msg object count = %d ",object_count);
                printf("lin_msg object_type = %d ",obj->object_type);
                printf("lin_msg object_flags = %d ",obj->object_flags);
                printf("lin_msg client_index = %d ",obj->client_index);
                printf("lin_msg object_version = %d ",obj->object_version);
                printf("lin_msg object_timestamp = %lld ",obj->object_timestamp);
                printf("lin_msg channel = %d ", obj->channel);
                printf("lin_msg dlc = %d ", obj->dlc);
                printf("lin_msg id = %d ", obj->id);
                printf("lin_msg dir = %d ", obj->dir);
                printf("lin_msg fsm_id = %d ", obj->fsm_id);
                printf("lin_msg fsm_state = %d ", obj->fsm_state);
                printf("lin_msg header_time = %d ", obj->header_time);
                printf("lin_msg full_time = %d ",obj->full_time);
                printf("lin_msg reserved: %d ",obj->reserved);
                for (int i = 0; i < obj->dlc; i++)
                {
                    printf("%d ",obj->data[i]);
                }
                printf("\n");
            }
            object_count++;
        }
    });

    getchar();
    printf("compression_level = %d\n",file_statistics_.compression_level);
    printf("file_size = %lld\n",file_statistics_.file_size);
    printf("object_count = %d\n",file_statistics_.object_count);
    printf("measurement_start_time = %d-%d-%d %d:%d:%d\n",file_statistics_.measurement_start_time.year,file_statistics_.measurement_start_time.month,
        file_statistics_.measurement_start_time.day,file_statistics_.measurement_start_time.hour,file_statistics_.measurement_start_time.minute,
        file_statistics_.measurement_start_time.second);
    printf("last_object_time = %d-%d-%d %d:%d:%d\n",file_statistics_.last_object_time.year,file_statistics_.last_object_time.month,
        file_statistics_.last_object_time.day,file_statistics_.last_object_time.hour,file_statistics_.last_object_time.minute,
        file_statistics_.last_object_time.second);
    printf("parse over object_count = %d\n", object_count);
    is_running = false;
    file.close();
    if (tid.joinable())
        tid.join();
    if (tid2.joinable())
        tid2.join();
    return 0;
}