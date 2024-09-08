#include <UnitTest++/UnitTestPP.h>

#include "../src/Exception.h"
#include "../src/Term.h"
#include "../src/Var.h"
#include "../src/Type.h"

SUITE(tsInit)
{
  TEST(Term_ByName) {
    Term t("name");
    CHECK_EQUAL("name", t.get_name());
    CHECK(&(t.get_type()) == &T);
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Type_ByName) {
    Type t("name");
    CHECK_EQUAL("name", t.get_name());
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Var_Top) {
    Var v("name", T);
    CHECK_EQUAL("name", v.get_name());
    CHECK(&(v.get_type()) == &T);
  }
  TEST(Var) {
    Type A("A");
    Var v("name", A);
    CHECK_EQUAL("name", v.get_name());
    CHECK(v.get_type() == A);
    CHECK(&(v.get_type()) != &A);
  }

  TEST(Term_ByNameType_Top) {
    Term t("name", T);
    CHECK_EQUAL("name", t.get_name());
    CHECK(&(t.get_type()) == &T);
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Term_ByNameType) {
    Type A("A");
    Term t("name", A);
    CHECK_EQUAL("name", t.get_name());
    CHECK(t.get_type() == A);
    CHECK(&(t.get_type()) != &A);
    CHECK_THROW(t[0], type_exception);
  }
}
