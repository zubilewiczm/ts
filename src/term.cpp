#include "exception.h"
#include "term.h"
#include "type.h"
#include "util.h"
#include "var.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>

static const auto null_deleter = [](const Type*) noexcept {};


Term::Term(const std::string& name, const Type& type, const Args& vars)
  : mName(name), mType(), mVars(vars.size())
{
  if (type != T) {
    mType = std::shared_ptr<const Type>(type.clone());
  }
  else {
    mType = std::shared_ptr<const Type>(&T, null_deleter);
  }
  std::transform(vars.begin(), vars.end(), mVars.begin(), [](const Term* t) {
    return std::shared_ptr<const Term>(t->clone());
  });

  auto tvars = mType->get_free_vars();
  auto argvars = traverse_free_vars();
  if (!std::includes(argvars.begin(), argvars.end(),
                     tvars.begin(), tvars.end()) )
  {
    std::stringstream err;
    err << "Free variables of "
        << mType->get_name()
        << " are not included as variables inside the term "
        << get_name() << ".";
    throw type_exception(err.str());
  }
}

Term::Term(const std::string& name, const Type& type)
  : Term(name, type, {}) {}

Term::Term(const std::string& name) : mName(name), mType(), mVars() {
  mType.reset(&T, null_deleter);
}

Term* Term::clone() const { return new Term(*this); }

Term* Term::deepcopy() const
{
  Term* copy = this->clone();
  copy->deepcopy_term_from(mVars);
  copy->mType = std::shared_ptr<const Type>(mType->deepcopy());
  return copy;
}

void Term::deepcopy_term_from(const ArgsStorage& src)
{
  std::transform(src.begin(), src.end(), mVars.begin(),
    [](const std::shared_ptr<const Term>& term) {
      return std::shared_ptr<const Term>(term->deepcopy());
    }
  );
}


std::unique_ptr<const Term, TermDeleter>
Term::subs(const Var& var, const Term& repl) const
{
  if (*this == T) {
    return std::unique_ptr<const Term, TermDeleter>(&T);
  }
  const SubsNode tree = this->subs_make_diff_tree(var);
  auto self_copy = std::unique_ptr<Term, TermDeleter>(this->clone());
  auto repl_copy = std::shared_ptr<Term>(repl.deepcopy());
  self_copy->subs_repl(var, repl_copy, tree);
  return self_copy;
}

Term::SubsNode
Term::subs_make_diff_tree(const Var& var) const
{
  SubsNode node;
  SubsNode next;
  node.subsP = SUBS_NO_CHANGE;
  if (*mType != T) {
    node.typeNode = std::make_unique<SubsNode>(mType->subs_make_diff_tree(var));
  }
  if (is_var() && *this == var) {
    node.subsP = SUBS_REPLACE;
  }
  else {
    for (const auto& term : mVars) {
      next = term->subs_make_diff_tree(var);
      if (next.subsP != SUBS_NO_CHANGE) {
        node.subsP = SUBS_OCCURS;
      }
      node.next.emplace_back(std::move(next));
    }
  }
  return node;
}

void
Term::subs_repl(const Var& var, const std::shared_ptr<const Term>& repl, const SubsNode& tree)
{
  auto term_i = mVars.begin();
  auto node_i = tree.next.begin();
  std::shared_ptr<Term> term_copy;
  std::shared_ptr<Type> type_copy;

  if (*mType != T && tree.typeNode->subsP != SUBS_NO_CHANGE) {
    type_copy = std::shared_ptr<Type>(mType->clone());
    type_copy->subs_repl(var, repl, *tree.typeNode);
    mType = type_copy;
  }
  while (term_i != mVars.end()) {
    switch (node_i->subsP) {
      case SUBS_REPLACE:
        *term_i = repl;
        break;
      case SUBS_OCCURS:
        term_copy.reset((*term_i)->clone());
        term_copy->subs_repl(var, repl, tree);
        *term_i = term_copy;
        break;
      case SUBS_NO_CHANGE:
        break;
    } 
    ++term_i; ++node_i;
  }
}

std::set<std::shared_ptr<const Var>>
Term::get_free_vars() const
{
  auto fvars = mType->get_free_vars();
  auto fvars_tree = traverse_free_vars();
  fvars.insert(fvars_tree.begin(), fvars_tree.end());
  return fvars;
}

std::set<std::shared_ptr<const Var>>
Term::traverse_free_vars() const
{
  std::set<std::shared_ptr<const Var>> fvars;
  std::set<std::shared_ptr<const Var>> other_fvars;
  for (const auto& el : mVars) {
    other_fvars = el->get_free_vars();
    fvars.insert(other_fvars.begin(), other_fvars.end());
  }
  return fvars;
}

bool
Term::has_free_var(const Var& v) const
{
  for (const auto& term : mVars) {
    if (term->has_free_var(v)) {
      return true;
    }
  }
  return mType->has_free_var(v);
}


std::string
Term::get_name() const
{
  std::string name = mName;
  if (!mVars.empty()) {
    name += "(";
    for (const auto& v : mVars) {
      name += v->get_name();
      name += ",";
    }
    name.back() = ')';
  }
  return name;
}

std::string
Term::get_name_and_type() const
{
  return get_name() + " : " + mType->get_name();
}

std::string
Term::get_uid() const
{
  auto uid = std::string(Term::PFX) + uid_escape(get_name());
  if (!mVars.empty()) {
    uid += "{";
    for (const auto& t : mVars) {
      uid += t->get_uid() + ",";
    }
    uid.back() = '}';
  }
  uid += ":" + mType->get_uid();
  return uid;
}


const Term&
Term::get_arg(int i) const
{
  int len = mVars.size();
  if (i >= len || i < 0) {
    std::stringstream ss;
    ss << "The term " << get_name() << " has no argument " << i;
    throw type_exception(ss.str());
  }
  else {
    return *mVars[i];
 }
}


bool
Term::is_type() const
{
  return *mType == T;
}


bool
operator==(const Term& lhs, const Term& rhs)
{
  return (lhs.get_uid() == rhs.get_uid());
}

bool
operator!=(const Term& lhs, const Term& rhs)
{
  return !(lhs == rhs);
}



void
TermDeleter::operator()(const Term* t)
{
  if (t != &T && t!= &N) {
    delete t;
  }
}
