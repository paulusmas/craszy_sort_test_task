#pragma once

#include "expression.h"

static int MAX_LEVEL = 1;
static int CUR_LEVEL = 0;

class Generator
{
public:
  Generator(int min_value, int max_value);
  bool run(const char* path, int countStrings);
  static const int MAX_VALUE;
  bool run();
protected:
  int getInt() const;
  char getSymbol() const;
  ItemPtr generateItem();
  ItemPtr subGenereteItem(ItemPtr pItem);
private:
  int m_min;
  int m_max;
  ItemPtr m_pAST;
};