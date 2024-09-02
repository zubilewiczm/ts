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

Term::Term(const Symbol& name, const Type& type, const ArgList& vars)
  : mName(name), mType(), mArgs(vars), mFreeVars(), PNameableWithStoredAlias()
{
  if (type != T) {
    mType = std::shared_ptr<const Type>(type.clone());
  }
  else {
    mType = Tptr;
  }

  auto tvars = mType->get_free_vars();
  auto argvars = mArgs.get_free_vars();
  if (!std::includes(argvars.begin(), argvars.end(),
                     tvars.begin(), tvars.end()) )
  {
    std::stringstream err;
    err << "Free variables of "
        << mType->get_name()
        << " are not included as variables inside the term "
        << get_name() << ".";
    throw type_exception(err.str());
  }
  mFreeVars = std::move(mArgs.get_free_vars());
}

Term::Term(const Symbol& name, const Type& type) : Term(name, type, {}) {}
Term::Term(const Symbol& name)
  : mName(name), mType(Tptr), mArgs(), mFreeVars(),
    PNameableWithStoredAlias() {}

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
    auto rfv = repl->get_free_vars();

    auto subs_result = std::shared_ptr<ITerm>(self_copy->mType->subs(var, repl));
    self_copy->mType = std::reinterpret_pointer_cast<Type>(subs_result);

    self_copy->mArgs.subs_inplace(var, repl);
    fv.erase(it);
    fv.insert(rfv.begin(), rfv.end());
  }
  return self_copy;
}

std::string Term::get_true_name() const
{
  return mName.get_name() + mArgs.get_name();
}

std::string Term::get_true_long_name() const
{
  return get_true_name() + " : " + mType->get_name();
}

std::string Term::get_uid() const
{
  auto uid = std::string(Term::PFX) + uid_escape(mName.get_true_name());
  uid += mArgs.get_uid_fragment();
  uid += ":" + mType->get_uid();
  return uid;
}


ArgList::TermPtr Term::operator[](std::size_t i) const
{
  int len = mArgs.size();
  if (i >= len || i < 0) {
    std::stringstream ss;
    ss << "The term " << get_name() << " has no argument " << i;
    throw type_exception(ss.str());
  }
  else {
    return mArgs[i];
 }
}
