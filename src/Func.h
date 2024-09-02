#ifndef _D_FUNC
#define _D_FUNC

#include "Term.h"
#include <cstring>
#include <memory>
#include <string>

class Func : public Term
{
  protected:
    Func(const std::string&, const Type&, const Term&, const Var&);
    Func(const Func&);

  public:
    std::string get_uid() const override;

  protected:
    std::unique_ptr<const Term, TermDeleter> apply(const Term&) const;

    Term mTerm;
    std::unique_ptr<const Var> mBoundVar;

  public:
    friend class Lambda;
};

#endif
