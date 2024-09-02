#include "IUIDComparable.h"

bool
operator==(const IUIDComparable& lhs, const IUIDComparable& rhs)
{
  return (lhs.get_uid() == rhs.get_uid());
}

bool
operator!=(const IUIDComparable& lhs, const IUIDComparable& rhs)
{
  return (lhs.get_uid() != rhs.get_uid());
}

bool
operator<(const IUIDComparable& lhs, const IUIDComparable& rhs)
{
  return (lhs.get_uid() < rhs.get_uid());
}

bool
operator<=(const IUIDComparable& lhs, const IUIDComparable& rhs)
{
  return (lhs.get_uid() <= rhs.get_uid());
}

bool
operator>(const IUIDComparable& lhs, const IUIDComparable& rhs)
{
  return (lhs.get_uid() > rhs.get_uid());
}

bool
operator>=(const IUIDComparable& lhs, const IUIDComparable& rhs)
{
  return (lhs.get_uid() >= rhs.get_uid());
}
