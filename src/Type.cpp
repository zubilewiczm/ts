#include "Type.h"

#include <memory>
#include <string>

#include "Util.h"

const Type T("Type");
const Type N("Empty");
const std::shared_ptr<const Type> Tptr(&T, [](const auto&){});


Type::Type(const Symbol& name) : Type(name, {}) {}
Type::Type(const Symbol& name, const ArgList& args)
  : mName(name), mArgs(args), mFreeVars(args.get_free_vars()),
    PNameableWithStoredAlias() {}

Type* Type::clone_impl() const { return new Type(*this); }
Type* Type::deepcopy_impl() const
{
  Type* copy = this->clone_impl();
  copy->mArgs = copy->mArgs.deepcopy();
  return copy;
}

std::string Type::get_true_name() const
{
  return mName.get_name();
}

std::string Type::get_true_long_name() const
{
  return mName.get_name() + " : " + T.get_true_name();
}

std::string Type::get_uid() const
{
  auto uid = std::string(Type::PFX) + uid_escape(mName.get_true_name());
  uid += mArgs.get_uid_fragment();
  return uid;
}

std::unique_ptr<ITerm>
Type::subs(const Var& var, const std::shared_ptr<const ITerm>& repl) const
{
  auto self_copy = clone();

  auto& fv = self_copy->mFreeVars;
  auto  it = self_copy->mFreeVars.find(var);
  
  if (it != fv.end()) {
    auto rfv = repl->get_free_vars();
    self_copy->mArgs.subs_inplace(var, repl);
    fv.erase(it);
    fv.insert(rfv.begin(), rfv.end());
  }
  return self_copy;
}
