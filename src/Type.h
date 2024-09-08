#ifndef _D_TYPE
#define _D_TYPE

#include "Term.h"

extern const Type T;
extern const Type N;
extern const std::shared_ptr<const Type> Tptr;

class Type :
  public IClonable<Type>::WithBase<ITerm>,
  public PNameableWithStoredAlias
{
  public:
    using ITerm::subs;

  public:
    constexpr static const char* const PFX = "τ";

  public:
    Type(const Symbol&);
    Type(const Symbol&, const ArgList&);
    
    std::string get_true_name_recursive() const override;
    std::string get_true_name() const override;
    std::string get_uid() const override;
    Symbol& get_head() override { return mName; }

    const Type& get_type() const override { return T; }
    SetOfVars get_free_vars() const override { return mFreeVars; }
    std::unique_ptr<ITerm> subs(const Var&, const std::shared_ptr<const ITerm>&) const override;

    bool is_type() const override { return true; }

    ArgList::TermPtr operator[](std::size_t i) const { return mArgs[i]; }

  protected:
    Type* clone_impl() const override;
    Type* deepcopy_impl() const override;

  private:
    Symbol mName;
    ArgList mArgs;
    SetOfVars mFreeVars;
};

#endif
