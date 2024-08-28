#include "var.h"
#include "type.h"
#include "term.h"
#include "util.h"

Var::Var(const std::string& name, const Type& t)
  : Term(name, t) {}

Var* Var::clone() const { return new Var(*this); }
Var* Var::deepcopy() const { return (Var*)Term::deepcopy(); }

std::set<std::shared_ptr<const Var>>
Var::get_free_vars() const
{
  return std::set<std::shared_ptr<const Var>>({std::shared_ptr<const Var>(this->clone())});
}

bool
Var::has_free_var(const Var& v) const
{
  return *this == v;
}

std::string
Var::get_name() const
{
  return mName;
}

std::string
Var::get_uid() const
{
  return std::string(Var::PFX) + uid_escape(mName) + ":" \
    + get_type().get_uid();
}
