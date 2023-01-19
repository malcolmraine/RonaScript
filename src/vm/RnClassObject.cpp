/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnClassObject.h"
#include <iomanip>
#include <sstream>

/*****************************************************************************/
auto RnClassObject::ToString() const -> RnStringNative
{
	std::stringstream s;
	s << "{class object @ " << std::hex << _data << "}";
	return s.str();
}
