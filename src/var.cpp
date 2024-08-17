#include "var.h"
#include "type.h"
#include "term.h"
#include "util.h"

Var::Var(const std::string& name, const Type& t)
  : Term(name, t) {}

Var* Var::clone() const { return new Var(*this); }
Var* Var::deepcopy() const { return (Var*)Term::deepcopy(); }

std::string
Var::get_name() const
{
  return mName;
}

std::string
Var::get_uid() const
{
  return std::string("σ") + uid_escape(mName) + ":" \
    + get_type().get_uid();
}
