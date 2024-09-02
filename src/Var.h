#ifndef _D_VAR
#define _D_VAR

#include <exception>
#include <string>
#include <memory>

#include "Symbol.h"

#include "interfaces/ITerm.h"
#include "interfaces/PNameableWithStoredAlias.h"

class Var :
  public IClonable<Var>::WithBase<ITerm>,
  public PNameableWithStoredAlias
{
  public:
    constexpr static const char* const PFX = "σ";

  public:
    Var(const Symbol&, const Type&);

    SetOfVars get_free_vars() const override { return SetOfVars({*this}); }
    bool has_free_var(const Var& v) const override { return v == *this; }

    bool is_var() const override { return true; }
    bool is_pure() const override { return true; }

    const Type& get_type() const override { return *mType; }
    std::unique_ptr<ITerm> subs(const Var&, const ITerm&) const override;
    std::unique_ptr<ITerm> subs(const Var&, const std::shared_ptr<const ITerm>&) const override;

    const std::shared_ptr<const ITerm>
      subs_as_arg(const Var&, const std::shared_ptr<const ITerm>&) const override;

    std::string get_true_name() const override;
    std::string get_true_long_name() const override;
    std::string get_uid() const override;

  protected:
    Var* clone_impl() const override;
    Var* deepcopy_impl() const override;

  private:
    Symbol mName;
    std::shared_ptr<const Type> mType;
};

#endif
