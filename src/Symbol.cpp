#include "Symbol.h"

#include <cstring>
#include <sstream>
#include <string>

#include "Exception.h"

Symbol::Symbol() : mName("?"), PNameableWithStoredAlias() {}
Symbol::Symbol(const char* name) : Symbol(std::string(name)) {}
Symbol::Symbol(const std::string& name) : mName(name), PNameableWithStoredAlias()
{
  if (!is_name_valid(mName)) {
    std::ostringstream msg;
    msg << "Symbol " << mName << "has a malformed name.";
    throw display_exception(msg.str());
  }
}

std::string Symbol::get_true_long_name() const
{
  return get_name();
}

std::string Symbol::get_true_name() const
{
  return mName;
}

bool Symbol::is_name_valid(const std::string& name)
{
  char forbidden[] = {
    '(', ')',
    '[', ']',
    ' ', '\t', '\n'
  };
 return name.find_first_of(forbidden) == std::string::npos;
}


bool operator==(const Symbol& lhs, const Symbol& rhs)
{
  return lhs.mName == rhs.mName;
}

bool operator!=(const Symbol& lhs, const Symbol& rhs)
{
  return lhs.mName != rhs.mName;
}
