#include "lambda.h"
#include "var.h"
#include "util.h"

#include <memory>
#include <string>

Lambda::Lambda(const Type& dom, const Type& codom)
  : mDomain(dom.clone()), mCodomain(codom.clone()),
    Type("Λ(" + dom.get_name() + "→" + codom.get_name() + ")") {}
Lambda::Lambda(const Lambda& other)
  : mDomain(other.mDomain->clone()),
    mCodomain(other.mCodomain->clone()),
    Type(other.mName) {}

Lambda& Lambda::operator=(const Lambda& rhs)
{
  if (this == & rhs) {
    return *this;
  }
  Type::operator=(rhs);
  mDomain = std::unique_ptr<const Type>(rhs.mDomain->clone());
  mCodomain = std::unique_ptr<const Type>(rhs.mCodomain->clone());
  return *this;
}

Lambda* Lambda::clone() const { return new Lambda(*this); }

Lambda*
Lambda::deepcopy() const
{
  Lambda* copy = (Lambda*)(this->Type::deepcopy());
  copy->mDomain   = std::unique_ptr<Type>(mDomain->deepcopy());
  copy->mCodomain = std::unique_ptr<Type>(mCodomain->deepcopy());
  return copy;
}


const Func
Lambda::cons(const Var& var, const Term& f, const char* n) const
{
  if (n != nullptr) {
    return Func(std::string(n), *this, f, var);
  }
  std::string name;
  name += "(λ";
  name += var.get_name();
  name += ").";
  name += f.get_name();
  return Func(name, *this, f, var);
}

const std::shared_ptr<Term>
Lambda::apply(const Func& f, const Term& x) const
{
  if (f.get_type() != *this) {
    throw f.get_name() + " is not of type " + get_name();
  }
  if (x.get_type() != *mDomain) {
    throw x.get_name() + " does not belong to the domain "\
      + mDomain->get_name() + " of " + get_name();
  }
  return f.apply(x);
}


std::string
Lambda::get_name() const
{
  std::string first, second;
  if (mDomain->get_name().find("→") != std::string::npos) {
    first += "(";
    first += mDomain->get_name();
    first += ")";
  } else {
    first += mDomain->get_name();
  }
  second = mCodomain->get_name();
  return first + " → " + second;
}

std::string
Lambda::get_uid() const
{
  return std::string("Λ") + uid_escape(mDomain->get_uid()) \
    + ">" +uid_escape(mCodomain->get_uid());
}


const Type&
Lambda::get_domain() const
{
  return *mDomain;
}

const Type&
Lambda::get_codomain() const
{
  return *mCodomain;
}
