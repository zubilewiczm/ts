#ifndef _D_INAMEABLE
#define _D_INAMEABLE

#include <string>

class INameable {
  public:
    virtual ~INameable() = default;

    virtual std::string get_name() const {
      return has_alias() ? get_alias() : get_true_name();
    }
    virtual std::string get_long_name() const {
      return has_alias() ? get_alias() : get_true_long_name();
    }

    virtual std::string get_true_long_name_recursive() const = 0;
    virtual std::string get_true_name_recursive() const = 0;
    virtual std::string get_true_long_name() const = 0;
    virtual std::string get_true_name() const = 0;
    virtual std::string get_alias() const = 0;
    virtual INameable& set_alias(const std::string&) = 0;

    INameable& drop_alias() { set_alias(""); return *this; }
    bool has_alias() const { return get_alias() != ""; }
};

std::ostream& operator<<(std::ostream& out, const INameable& rhs);

#endif
