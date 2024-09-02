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
