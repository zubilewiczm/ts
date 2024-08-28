#include "type.h"

#include <memory>
#include <string>

#include "exception.h"
#include "util.h"

const Type T("Type");
const Type N("Empty");

Type::Type(const std::string& name) : Term(name) {}
Type::Type(const std::string& name, const Args& args) : Term(name, T, args) {}
Type::Type(const Type& t) : Type(t.mName) {}
Type* Type::clone() const { return new Type(*this); }
Type* Type::deepcopy() const
{
  Type* copy = this->clone();
  copy->deepcopy_term_from(mVars);
  return copy;
}

std::set<std::shared_ptr<const Var>>
Type::get_free_vars() const
{
  return traverse_free_vars();
}

bool
Type::has_free_var(const Var& v) const
{
  for (const auto& term : mVars) {
    if (term->has_free_var(v)) {
      return true;
    }
  }
  return false;
}

std::string
Type::get_uid() const
{
  auto uid = std::string(Type::PFX) + uid_escape(get_name());
  if (!mVars.empty()) {
    uid += "{";
    for (const auto& t : mVars) {
      uid += t->get_uid() + ",";
    }
    uid.back() = '}';
  }
  return uid;
}

const Type&
Type::get_type() const
{
  return T;
}

std::unique_ptr<const Type, TermDeleter>
Type::cons(const Term& v)
{
  if (v.get_type() == T) {
    if (v == T || v.get_uid() == std::string(Term::PFX) + "Type") {
      return std::unique_ptr<const Type, TermDeleter>(&T);
    }
    else {
      return std::make_unique<const Type>(get_name());
    }
  }
  throw type_exception(v.get_name() + " is not a type");
}
