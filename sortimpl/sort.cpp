#include "stdafx.h"
#include <vector>
#include "expression.h"
void API_EXPORT A(const std::string& in, std::string& out)
{
  if (in.empty()) return;
  try
  {
    Expression expr;
    expr.parse(in);
    expr.sort<ItemLessPredicate>();
    expr.getString(out);
  }
  catch (std::exception& e)
  {
    out = "String is not sorted. Syntax error: " + in;
  }
}