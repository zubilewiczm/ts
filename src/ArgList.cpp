#include "ArgList.h"

#include "Exception.h"
#include "Var.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

ArgList::ArgList(const TermVec& args) : mArgs(args) {}
ArgList::ArgList(TermVec&& args) : mArgs(std::move(args)) {}

ArgList::ArgList(std::initializer_list<PtrArg<ITerm>> args)
  : mArgs()
{
  std::transform(args.begin(), args.end(), std::back_inserter(mArgs),
    [&](const auto& a) {
      return a.get();
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

std::string ArgList::get_true_name() const
{
  return compose_str("(", ")", [](const TermPtr& a) -> std::string {
    return a->get_true_name_recursive();
  });
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
      term_i = term_i->subs_as_arg(v, t);
    }
  }
}

ArgList::TermPtr ArgList::operator[](std::size_t i) const
{
  int len = mArgs.size();
  if (i >= len || i < 0) {
    std::stringstream ss;
    ss << "The term " << get_name() << " has no argument " << i;
    throw type_exception(ss.str());
  }
  else {
    return mArgs[i];
  }
}
