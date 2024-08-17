#ifndef _D_TERM
#define _D_TERM

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class Var;
class Term;
class Type;
typedef std::vector<const Term*> Args;
typedef std::vector<std::shared_ptr<const Term> > ArgsStorage;

class Term
{
  public:
    Term(const std::string&);
    Term(const std::string&, const Type&);
    Term(const std::string&, const Type&, const Args&);
    virtual ~Term() = default;
    virtual Term* clone() const;
    virtual Term* deepcopy() const;

    virtual std::string get_name() const;
    std::string get_name_and_type() const;
    virtual std::string get_uid() const;
    virtual const Type& get_type() const;
    const std::string& get_head() const;

    std::shared_ptr<Term> subs(const Var&, const Term&) const;
    const Term& get_arg(int i) const;

    friend bool operator==(const Term&, const Term&);
    friend bool operator!=(const Term&, const Term&);

  protected:
    enum SubsWillChange {
      SUBS_REPLACE = 3,
      SUBS_OCCURS = 1,
      SUBS_NO_CHANGE = 0
    };

    struct SubsNode {
      std::vector<SubsNode> next;
      std::unique_ptr<SubsNode> typeNode;
      SubsWillChange subsP;
    };

    SubsNode subs_make_diff_tree(const Var&) const;
    void subs_repl(const Var&, const std::shared_ptr<const Term>&, const SubsNode&);

    void deepcopy_term_from(const ArgsStorage&);

    std::string mName;
    std::shared_ptr<const Type> mType;
    ArgsStorage mVars;
};

#endif
