#ifndef _D_VAR
#define _D_VAR

#include <exception>
#include <string>
#include <memory>

#include "type.h"

class Var : public Term
{
  public:
    constexpr static const char* const PFX = "σ";

  public:
    Var(const std::string&, const Type&);
    Var* clone() const override;
    Var* deepcopy() const override;

    std::set<std::shared_ptr<const Var>> get_free_vars() const override;
    bool has_free_var(const Var&) const override;

    bool is_var() const override { return true; }

    std::string get_name() const override;
    std::string get_uid() const override;
};

#endif
