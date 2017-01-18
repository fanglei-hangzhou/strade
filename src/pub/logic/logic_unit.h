/*
 * logic_unit.h
 *
 *  Created on: 2014年12月2日
 *      Author: kerry
 */
#ifndef _BABY_LOGIC_UNIT_H__
#define _BABY_LOGIC_UNIT_H__

#include <string>
#include "lbs/lbs_connector.h"
#include "logic/logic_comm.h"
#include "net/comm_head.h"
#include "net/http_data.h"
#include "basic/native_library.h"
#include <string>

namespace base_logic {

class SomeUtils {
 public:
  static void* GetLibraryFunction(const std::string& library_name,
                                  const std::string& func_name);
};

class LogicUnit {
 public:
  static void SendMessageBySize(const int socket, std::string& json);
  static void CreateToken(const int64 uid, std::string& token);

};
}

#define send_message_by_size  base_logic::LogicUnit::SendMessageBySize

#endif

