#ifndef _D_TYPE
#define _D_TYPE

#include "term.h"

extern const Type T;
extern const Type N;

class Type : public Term
{
  public:
    constexpr static const char* const PFX = "τ";

  public:
    Type(const std::string&);
    Type(const std::string&, const Args&);
    Type(const Type&);
    Type* clone() const override;
    Type* deepcopy() const override;
    
    std::set<std::shared_ptr<const Var>> get_free_vars() const override;
    bool has_free_var(const Var&) const override;

    const Type& get_type() const override;
    std::string get_uid() const override;
    std::unique_ptr<const Type, TermDeleter> cons(const Term&);
};

#endif
