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


std::shared_ptr<Term>
Term::subs(const Var& var, const Term& repl) const
{
  const SubsNode tree = this->subs_make_diff_tree(var);
  auto self_copy = std::shared_ptr<Term>(this->clone());
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
  if (mVars.empty() && *this == var) {
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
  auto uid = std::string("ρ") + uid_escape(get_name());
  if (mVars.empty()) {
    uid += "{";
    for (const auto& t : mVars) {
      uid += t->get_uid() + ",";
    }
    uid[-1] = '}';
  }
  uid += ":" + mType->get_uid();
  return uid;
}

const Type&
Term::get_type() const
{
  return *mType;
}

const std::string&
Term::get_head() const
{
  return mName;
}


const Term&
Term::get_arg(int i) const
{
  int len = mVars.size();
  if (i >= len || i < 0) {
    std::stringstream ss;
    ss << "The term " << get_name() << " has no argument " << i;
    throw ss.str();
  }
  else {
    return *mVars[i];
  }
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
