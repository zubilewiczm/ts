#ifndef _D_LAMBDA
#define _D_LAMBDA

#include "Type.h"
#include "Func.h"
#include <memory>

class Lambda : public Type
{
  public:
    Lambda(const Type&, const Type&);
    Lambda(const Lambda&);
    Lambda& operator=(const Lambda&);
    Lambda* clone() const override;
    Lambda* deepcopy() const override;

    const Func cons(const Var&, const Term&, const char* = nullptr) const;
    const std::unique_ptr<const Term, TermDeleter> apply(const Func&, const Term&) const;

    std::string get_name() const override;
    std::string get_uid() const override;
    const Type& get_domain() const;
    const Type& get_codomain() const;

  protected:
    std::unique_ptr<const Type> mDomain;
    std::unique_ptr<const Type> mCodomain;
};

#endif
