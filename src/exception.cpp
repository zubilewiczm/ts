#include "exception.h"

type_exception::type_exception(const char* msg) : mMsg(msg) {}
type_exception::type_exception(std::string msg) : mMsg(msg) {}

const char*
type_exception::what() const noexcept
{
  return mMsg.data();
}
