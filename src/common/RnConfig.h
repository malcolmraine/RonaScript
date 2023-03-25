/*****************************************************************************
* File: RnConfig.h
* Description:
* Author: Malcolm Hall
* Date: 3/24/23
* Version: 1
*
*****************************************************************************/

#ifndef RONASCRIPT_SRC_COMMON_RNCONFIG_H_
#define RONASCRIPT_SRC_COMMON_RNCONFIG_H_

#include <string>

class RnConfig {
public:
    static std::string GetLibraryPath();
};

#endif  //RONASCRIPT_SRC_COMMON_RNCONFIG_H_