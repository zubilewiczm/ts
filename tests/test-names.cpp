#include <UnitTest++/UnitTestPP.h>
#include <string>

#include "../src/Exception.h"
#include "../src/Term.h"
#include "../src/Var.h"
#include "../src/Type.h"

SUITE(tsNames)
{
  constexpr const char* weird_name = "aAgE41@$%^$$?!~%@-/\\.,∔∑∁∊";
  constexpr const char* bad_name_space = " ";
  constexpr const char* bad_name_lbrace = "(";
  constexpr const char* bad_name_rbrace = ")";
  constexpr const char* bad_name_lsbrace = "[";
  constexpr const char* bad_name_rsbrace = "]";
  constexpr const char* bad_name_tab = "\t";
  constexpr const char* bad_name_nl = "\n";

  TEST(Term_GetName)
  {
    Term t(weird_name, T);
    CHECK_EQUAL(weird_name, t.get_name());
  }

  TEST(Term_GetName_ForbiddenSpace)
  {
    CHECK_THROW(Term t(bad_name_space, T), display_exception);
  }

  TEST(Term_GetName_ForbiddenLBrace)
  {
    CHECK_THROW(Term t(bad_name_lbrace, T), display_exception);
  }

  TEST(Term_GetName_ForbiddenRBrace)
  {
    CHECK_THROW(Term t(bad_name_rbrace, T), display_exception);
  }

  TEST(Term_GetName_ForbiddenLSBrace)
  {
    CHECK_THROW(Term t(bad_name_lsbrace, T), display_exception);
  }

  TEST(Term_GetName_ForbiddenRSBrace)
  {
    CHECK_THROW(Term t(bad_name_rsbrace, T), display_exception);
  }

  TEST(Term_GetName_ForbiddenTab)
  {
    CHECK_THROW(Term t(bad_name_tab, T), display_exception);
  }

  TEST(Term_GetName_ForbiddenNL)
  {
    CHECK_THROW(Term t(bad_name_nl, T), display_exception);
  }


  TEST(Term_GetLongName)
  {
    Term t(weird_name, T);
    CHECK_EQUAL(std::string(weird_name) + " : ⊤", t.get_long_name());
  }
  TEST(Type_GetLongName)
  {
    Type t("A");
    CHECK_EQUAL("A : ⊤", t.get_long_name());
  }

  TEST(TermWithType_GetName)
  {
    Type t("A");
    Term term("phi", t);
    CHECK_EQUAL("phi", term.get_name());
  }
  TEST(TermWithType_GetLongName)
  {
    Type t("A");
    Term term("phi", t);
    CHECK_EQUAL("phi : A", term.get_long_name());
  }

  TEST(VarWithType_GetName)
  {
    Type t("A");
    Var v("a", t);
    CHECK_EQUAL("a", v.get_name());
  }
  TEST(VarWithType_GetLongName)
  {
    Type t("A");
    Var v("a", t);
    CHECK_EQUAL("a : A", v.get_long_name());
  }

  
  struct TermWithArgsFixture {
    Term c;
    Var x;
    Var y;
    Type t;
    Type A;
    Term phi;

    TermWithArgsFixture()
      : c("c"), x("x", T), y("y", T), t("T"), A("A", {x}), phi("φ", A, {c,x,y,t}) {}
  };

  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_GetName)
  {
    CHECK_EQUAL("φ(c,x,y,T)", phi.get_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_GetLongName)
  {
    CHECK_EQUAL("φ(c,x,y,T) : A(x)", phi.get_long_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_GetName)
  {
    phi.set_alias("huh");
    CHECK_EQUAL("huh", phi.get_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_GetLongName)
  {
    phi.set_alias("bob");
    CHECK_EQUAL("bob : A(x)", phi.get_long_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_Reset_GetName)
  {
    phi.set_alias("huh");
    phi.set_alias("");
    CHECK_EQUAL("φ(c,x,y,T)", phi.get_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_Head_GetName)
  {
    phi.get_head().set_alias("huh");
    CHECK_EQUAL("huh(c,x,y,T)", phi.get_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_Type_GetLongName)
  {
    A.set_alias("huh");
    phi = Term("φ", A, {c,x,y,t});
    CHECK_EQUAL("φ(c,x,y,T) : huh", phi.get_long_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_Type_Head_GetLongName)
  {
    A.get_head().set_alias("huh");
    phi = Term("φ", A, {c,x,y,t});
    CHECK_EQUAL("φ(c,x,y,T) : huh(x)", phi.get_long_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_Var_GetLongName)
  {
    x.set_alias("X X X");
    A = Type("A", {x});
    A.get_head().set_alias("huh");
    phi = Term("φ", A, {c,x,y,t});
    CHECK_EQUAL("φ(c,X X X,y,T) : huh(X X X)", phi.get_long_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_GetTrueName)
  {
    phi.set_alias("huh");
    CHECK_EQUAL("φ(c,x,y,T)", phi.get_true_name());
  }
  TEST_FIXTURE(TermWithArgsFixture,
      TermWithArgs_Aliased_GetTrueLongName)
  {
    phi.set_alias("bob");
    CHECK_EQUAL("φ(c,x,y,T) : A(x)", phi.get_true_long_name());
  }


  struct TermWithArgsFixtureAliased {
    Term c;
    Var x;
    Var y;
    Type t;
    Type A;
    Term phi;

    TermWithArgsFixtureAliased()
      : c("c"), x("x", T), y("y", T), t("T"), A("A", {x}), phi("φ", A, {c,x,y,t})
    {
      x.set_alias("X");
      A = Type("A",{x});
      A.get_head().set_alias("B");
      phi = Term("φ", A, {c,x,y,t});
      phi.set_alias("bob");
    }
  };
  TEST_FIXTURE(TermWithArgsFixtureAliased,
      TermWithArgs_Aliased_Var_GetTrueLongName)
  {
    CHECK_EQUAL("φ(c,X,y,T) : B(X)", phi.get_true_long_name());
  }
  TEST_FIXTURE(TermWithArgsFixtureAliased,
      TermWithArgs_Aliased_Var_GetTrueNameRec)
  {
    CHECK_EQUAL("φ(c,x,y,T)", phi.get_true_name_recursive());
  }
  TEST_FIXTURE(TermWithArgsFixtureAliased,
      TermWithArgs_Aliased_Var_GetTrueLongNameRec)
  {
    CHECK_EQUAL("φ(c,x,y,T) : A(x)", phi.get_true_long_name_recursive());
  }
}
