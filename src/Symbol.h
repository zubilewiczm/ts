#ifndef _D_SYMBOL
#define _D_SYMBOL

#include <string>

#include "interfaces/PNameableWithStoredAlias.h"

class Symbol : public PNameableWithStoredAlias
{
  public:
    Symbol();
    Symbol(const char*);
    Symbol(const std::string&);

    std::string get_true_name() const override;
    std::string get_true_long_name() const override;
    std::string get_true_name_recursive() const override;
    std::string get_true_long_name_recursive() const override;

  protected:
    static bool is_name_valid(const std::string&);
    
  private:
    std::string mName;

  public:
    friend bool operator==(const Symbol&, const Symbol&);
    friend bool operator!=(const Symbol&, const Symbol&);
};

#endif
