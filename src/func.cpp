#include "func.h"
#include "var.h"
#include "util.h"
#include <memory>

Func::Func(
    const std::string& name,
    const Type& t,
    const Term& term,
    const Var& v
    )
  : Term(name, t, {}), mTerm(term), mBoundVar(v.clone()) {}

Func::Func(const Func& other)
  : Term(other), mTerm(other.mTerm), \
      mBoundVar(other.mBoundVar->clone()) {}


std::string
Func::get_uid() const
{
  return std::string("λ") + uid_escape(mName);
}

std::shared_ptr<Term>
Func::apply(const Term& x) const
{
  return mTerm.subs(*mBoundVar, x);
}
