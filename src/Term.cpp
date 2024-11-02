#include "Exception.h"
#include "Term.h"
#include "Type.h"
#include "Util.h"
#include "Var.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>

Term::Term(const Symbol& name)
  : Term(name, Tptr, {}) {}
Term::Term(const Symbol& name, PtrArgType type, const ArgList& args)
  : mName(name),
    mType(type),
    mArgs(args),
    mFreeVars(args.get_free_vars()),
    PNameableWithStoredAlias()
{
  auto tvars = mType->get_free_vars();
  if (!std::includes(mFreeVars.begin(), mFreeVars.end(),
                     tvars.begin(), tvars.end()) )
  {
    std::stringstream err;
    err << "Free variables of "
        << mType->get_name()
        << " (namely " << tvars << ") "
        << "are not included as variables inside the term " << get_name()
        << " (as " << mFreeVars << ").";
    throw type_exception(err.str());
  }
}

Term* Term::clone_impl() const { return new Term(*this); }
Term* Term::deepcopy_impl() const
{
  Term* copy{this->clone_impl()};
  copy->mArgs = copy->mArgs.deepcopy();
  copy->mType = std::shared_ptr<const Type>(mType->deepcopy());
  return copy;
}



std::unique_ptr<ITerm>
Term::subs(const Var& var, const std::shared_ptr<const ITerm>& repl) const
{
  auto self_copy = clone();

  auto& fv = self_copy->mFreeVars;
  auto  it = fv.find(var);
  
  if (it != fv.end()) {
    if (self_copy->mType->has_free_var(var)) {
      auto type_ptr = std::static_pointer_cast<const ITerm>(self_copy->mType);
      auto subs_type = type_ptr->subs_as_arg(var, repl);
      self_copy->mType = std::static_pointer_cast<const Type>(subs_type);
    }

    self_copy->mArgs.subs_inplace(var, repl);
    fv.erase(it);

    auto rfv = repl->get_free_vars();
    fv.insert(rfv.begin(), rfv.end());
  }
  return self_copy;
}



std::string Term::get_true_name_recursive() const
{
  return mName.get_true_name() + mArgs.get_true_name();
}

std::string Term::get_true_name() const
{
  return mName.get_name() + mArgs.get_name();
}

std::string Term::get_uid() const
{
  auto uid = std::string(Term::PFX) + uid_escape(mName.get_true_name());
  uid += mArgs.get_uid_fragment();
  uid += ":" + mType->get_uid();
  return uid;
}

