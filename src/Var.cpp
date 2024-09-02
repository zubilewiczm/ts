#include "Var.h"
#include "Type.h"
#include "Util.h"
#include <memory>
#include <string>

Var::Var(const Symbol& name, const Type& t)
  : mName(name), mType(t.clone()) {}

Var* Var::clone_impl() const
{
  return new Var(*this);
}

Var* Var::deepcopy_impl() const {
  Var* copy = clone_impl();
  copy->mType = copy->mType->deepcopy();
  return copy;
}


std::string Var::get_true_name() const
{
  return mName.get_name();
}

std::string Var::get_true_long_name() const
{
  return get_true_name() + " : " + mType->get_name();
}

std::string Var::get_uid() const
{
  return std::string(Var::PFX) + uid_escape(mName.get_true_name()) + ":"
    + get_type().get_uid();
}


std::unique_ptr<ITerm>
Var::subs(const Var& var, const ITerm& repl) const
{
  return has_free_var(var) ? repl.deepcopy() : clone();
}

std::unique_ptr<ITerm>
Var::subs(const Var& var, const std::shared_ptr<const ITerm>& repl) const
{
  return has_free_var(var) ? repl->deepcopy() : clone();
}
