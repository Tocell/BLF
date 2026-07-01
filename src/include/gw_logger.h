#ifndef GW_LOGGER_H
#define GW_LOGGER_H

#include "bus_message.h"
#include "can_object.h"
#include "ethernet_object.h"
#include "flexray_object.h"
#include "frame_type_define.h"
#include "lin_object.h"
#include "logger.h"
#include "message_factory.h"

#include "can/can_driver_err_ext_message.h"
#include "can/can_driver_err_message.h"
#include "can/can_driver_sync_message.h"
#include "can/can_err_ext_message.h"
#include "can/can_error_message.h"
#include "can/can_message.h"
#include "can/can_message2.h"
#include "can/can_overload_message.h"
#include "can/can_setting_change_message.h"
#include "can/can_statistic_message.h"
#include "can/canfd_err64_message.h"
#include "can/canfd_message.h"
#include "can/canfd_message64.h"
#include "ethernet/ethernet_message.h"
#include "flexray/flexray_message.h"
#include "lin/lin_message.h"

namespace gwlogger = GWLogger;

#endif // GW_LOGGER_H
