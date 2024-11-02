#ifndef _D_ARGLIST
#define _D_ARGLIST

#include "interfaces/ITerm.h"
#include "utils/PtrArg.h"

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

class ArgList
{
  public:
    using TermPtr = std::shared_ptr<const ITerm>;
    using TermPtrArg = PtrArg<ITerm>;
    using TermVec = std::vector<TermPtr>;

    using arg_str_gen = std::string (*)(const TermPtr&);

  public:
    ArgList() = default;
    ArgList(const TermVec&);
    ArgList(TermVec&&);
    ArgList(std::initializer_list<TermPtrArg>);

    std::string get_name() const;
    std::string get_true_name() const;
    std::string get_uid_fragment() const;

    void subs_inplace(const Var&, const TermPtr&);
    SetOfVars get_free_vars() const;
    ArgList deepcopy();

    TermPtr operator[](std::size_t p) const;
    std::size_t size() const { return mArgs.size(); }

  protected:
    std::string compose_str(const char*, const char*, arg_str_gen) const;

  private:
    TermVec mArgs;
};

#endif
