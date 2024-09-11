#include "INameable.h"

std::ostream& operator<<(std::ostream& out, const INameable& rhs)
{
  out << rhs.get_name();
  return out;
}
