#include "ArgList.h"

#include "Var.h"

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <string>

ArgList::ArgList() : mArgs() {}
ArgList::ArgList(const TermVec& args) : mArgs(args) {}
ArgList::ArgList(const TermVec&& args) : mArgs(std::move(args)) {}
ArgList::ArgList(const std::initializer_list<std::reference_wrapper<ITerm>>& args) 
  : mArgs()
{
  std::transform(args.begin(), args.end(), std::back_inserter(mArgs),
    [&](const auto& a) {
      return a.get().clone();
    });
}

ArgList ArgList::deepcopy()
{
  TermVec tgt;
  std::transform(mArgs.begin(), mArgs.end(), std::back_inserter(tgt),
    [](const TermPtr& term) {
      return TermPtr(term->deepcopy());
    }
  );
  return tgt;
}

std::string ArgList::get_name() const
{
  return compose_str("(", ")", [](const TermPtr& a) -> std::string {
    return a->get_name();
  });
}

std::string ArgList::get_uid_fragment() const
{
  return compose_str("{", "}", [](const TermPtr& a) -> std::string {
    return a->get_uid();
  });
}

std::string ArgList::compose_str(const char* lb, const char* rb, arg_str_gen callback) const
{
  if (mArgs.empty()) {
    return "";
  }
  std::string name(lb);
  for (const auto& a : mArgs) {
    name += callback(a);
    name += ",";
  }
  name.pop_back();
  name.append(rb);
  return name;
}

SetOfVars ArgList::get_free_vars() const
{
  SetOfVars fvars;
  SetOfVars other_fvars;
  for (const auto& el : mArgs) {
    other_fvars = el->get_free_vars();
    fvars.insert(other_fvars.begin(), other_fvars.end());
  }
  return fvars;
}

void ArgList::subs_inplace(const Var& v, const TermPtr& t)
{
  for (auto& term_i : mArgs) {
    if (term_i->has_free_var(v)) {
      term_i = term_i->subs(v, t);
    }
  }
}
