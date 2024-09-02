#ifndef _D_ITERM
#define _D_ITERM

#include <algorithm>
#include <functional>
#include <memory>
#include <set>
#include <type_traits>

#include "INameable.h"
#include "IUIDComparable.h"
#include "IClonable.h"

class Var;
class Type;

using VarRef = std::reference_wrapper<const Var>;
using VarPtr = std::shared_ptr<const Var>;

struct SetOfVars_cmp {
  bool operator()(const VarRef&, const VarRef&) const;
};

using SetOfVars = std::set<VarRef, SetOfVars_cmp>;


class ITerm :
  virtual public INameable,
  virtual public IUIDComparable,
  public IClonable<ITerm>
{
  public:
    virtual std::unique_ptr<ITerm> subs(const Var& v, const ITerm& r) const {
      auto c = std::shared_ptr<ITerm>(r.deepcopy()); return subs(v, c);
    }
    virtual std::unique_ptr<ITerm> subs(const Var&, const std::shared_ptr<const ITerm>&) const = 0;

    virtual const Type& get_type() const = 0;
    virtual SetOfVars get_free_vars() const = 0;
    virtual bool has_free_var(const Var& v) const {
      auto s = get_free_vars(); return s.find(v) != s.end();
    }

    virtual bool is_pure() const { return get_free_vars().empty(); }
    virtual bool is_const() const { return false; }
    virtual bool is_var() const { return false; }
    virtual bool is_type() const;
};

#endif
