#ifndef _D_TERM
#define _D_TERM

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <set>
#include <type_traits>
#include <vector>

#include "Type.h"
#include "interfaces/IClonablePtrFactory.h"
#include "interfaces/IUIDComparable.h"
#include "interfaces/PNameableWithStoredAlias.h"
#include "utils/PtrArg.h"

#include "ArgList.h"
#include "Symbol.h"

class Term :
  public IClonablePtrFactory<Term, ITerm>,
  public PNameableWithStoredAlias
{
  public:
    using ITerm::subs;

  public:
    constexpr static const char* const PFX = "ρ";

  public:
    Term(const Symbol&);
    Term(const Symbol&, PtrArgType, const ArgList& = {});
    virtual ~Term() = default;

    std::string get_true_name_recursive() const override;
    std::string get_true_name() const override;
    std::string get_uid() const override;
    Symbol& get_head() override { return mName; }

    const Type& get_type() const override { return *mType;}
    SetOfVars get_free_vars() const override { return mFreeVars; }
    std::unique_ptr<ITerm> subs(const Var&, const std::shared_ptr<const ITerm>&) const override;

    ArgList::TermPtr operator[](std::size_t i) const { return mArgs[i]; }

  protected:
    Term* clone_impl() const override;
    Term* deepcopy_impl() const override;

  private:
    Symbol mName;
    std::shared_ptr<const Type> mType;
    ArgList mArgs;
    SetOfVars mFreeVars;
};

#endif
