#include "type.h"
#include "util.h"

const Type T("Type");
const Type N("Empty");

Type::Type(const std::string& name) : Term(name) {}
Type::Type(const Type& t) : Type(t.mName) {}
Type* Type::clone() const { return new Type(*this); }
Type* Type::deepcopy() const
{
  Type* copy = this->clone();
  copy->deepcopy_term_from(mVars);
  return copy;
}

std::string
Type::get_name() const
{
  return mName;
}

std::string
Type::get_uid() const
{
  return std::string("τ") + uid_escape(mName);
}

const Type&
Type::get_type() const
{
  return T;
}

const Type&
Type::cons(const Term& v)
{
  const Type* ptr = nullptr;
  if (v.get_type() == T) {
    ptr = dynamic_cast<const Type*>(this);
  }
  if (ptr == nullptr) {
    throw v.get_name() + " is not a type";
  }
  return *ptr;
}
