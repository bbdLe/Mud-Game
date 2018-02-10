/*
 * @Author: bbdle 
 * @Date: 2018-02-09 00:20:02 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 21:07:43
 */

#include "MUDLogs.h"

namespace MUD
{
    BasicLib::TextLog ERRORLOG("/home/mud_game/Libraries/Log/errors.log", "Error Log", true, true);
    BasicLib::TextLog USERLOG("/home/mud_game/Libraries/Log/users.log", "User Log", true, true);
}