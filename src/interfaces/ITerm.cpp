#include "ITerm.h"

#include "../Type.h"
#include "../Var.h"

bool SetOfVars_cmp::operator()(const VarRef& lhs, const VarRef& rhs) const
{
  const Var& lr = lhs.get();
  const Var& rr = rhs.get();
  return lr < lr;
}

bool ITerm::is_type() const {
  return get_type() == T;
}

std::string ITerm::get_long_name() const
{
  return get_name() + " : " + get_type().get_name();
}

std::string ITerm::get_true_long_name() const
{
  return get_true_name() + " : " + get_type().get_true_name();
}

std::string ITerm::get_true_long_name_recursive() const
{
  return get_true_name_recursive() + " : " + get_type().get_true_name_recursive();
}
