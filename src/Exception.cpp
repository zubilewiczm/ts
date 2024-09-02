#include "Exception.h"

generic_exception::generic_exception(const char* msg) : mMsg(msg) {}
generic_exception::generic_exception(std::string msg) : mMsg(msg) {}

const char*
generic_exception::what() const noexcept
{
  return mMsg.data();
}
