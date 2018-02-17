#include "stdafx.h"
#include <algorithm>
#include <stack>
#include <exception>

#include "expression.h"

#define MSG_INVALID_STRING "Invalid string"

bool Item::isValue() const
{
  return m_value != 0;
}

bool Item::isVector() const
{
  return !m_chunk.empty();
}

const ArrayItemPtr& Item::chunk() const
{
  return m_chunk;
}

ArrayItemPtr& Item::chunk()
{
  return m_chunk;
}

unsigned int Item::logicalLength() const
{
  return subLogicalLength(*this);
}

unsigned int Item::subLogicalLength(const Item& rhs) const
{
  unsigned int ret = 0;
  if (rhs.isValue())
    return 1;
  else if (rhs.isVector())
  {
    for(ArrayItemPtr::const_iterator b = rhs.chunk().begin(), e = rhs.chunk().end(); b != e; ++b)
    {
      ret += subLogicalLength(*(*b));
    }
  }
  return ret;
}

bool Item::subLess(const Item& rhs) const
{
  if (isValue() && rhs.isValue())
  {
    return getValue() < rhs.getValue();
  }
  else if (isValue() && rhs.isVector())
  {
    ItemPtr pItem = *rhs.chunk().begin();
    bool isEqual = *this == *pItem;
    if (isEqual)
      return true;
    return operator < (*pItem);
  }
  else if (isVector() && rhs.isValue())
  {
    ItemPtr pItem = *chunk().begin();
    bool isEqual = *pItem == rhs;
    if (isEqual)
      return false;   
    return pItem->operator <( rhs );
  }
  else if (isVector() && rhs.isVector())
  {
    ArrayItemPtr::const_iterator  b1 = chunk().begin(),
                                  b2 = rhs.chunk().begin(),
                                  e1 = chunk().end(),
                                  e2 = rhs.chunk().end();
    for( ; b1 != e1 && b2 != e2; b1++, b2++)
    {
      if (*(*b1) == *(*b2))
        continue;
      return *(*b1) < *(*b2);
    }
    long l1 = logicalLength();
    long l2 = rhs.logicalLength();
    return l1 < l2;
  }
  return false;
}

bool Item::subGreater(const Item& rhs) const
{
  if (isValue() && rhs.isValue())
  {
    return getValue() > rhs.getValue();
  }
  else if (isValue() && rhs.isVector())
  {
    ItemPtr pItem = *rhs.chunk().begin();
    bool isEqual = *this == *pItem;
    if (isEqual)
      return false;
    return operator > (*pItem);
  }
  else if (isVector() && rhs.isValue())
  {
    ItemPtr pItem = *chunk().begin();
    bool isEqual = *pItem == *this;
    if (!isEqual)
      return true;
    return pItem->operator >( rhs );
  }
  else if (isVector() && rhs.isVector())
  {
    ArrayItemPtr::const_iterator  b1 = chunk().begin(),
                                  b2 = rhs.chunk().begin(),
                                  e1 = chunk().end(),
                                  e2 = rhs.chunk().end();
    for( ; b1 != e1 && b2 != e2; b1++, b2++)
    {
      if (*(*b1) == *(*b2))
        continue;
      return *(*b1) > *(*b2);
    }
    long l1 = logicalLength();
    long l2 = rhs.logicalLength();
    return l1 > l2;
  }
  return false;
}

bool Item::subEqual(const Item& rhs) const
{
  if (isValue() && rhs.isValue())
  {
    return getValue() == rhs.getValue();
  }
  else if (isValue() && rhs.isVector())
  {
    bool ret = operator ==( *(*rhs.chunk().begin()) );
    return ret;
  }
  else if (isVector() && rhs.isValue())
  {
    return (*chunk().begin())->operator ==( rhs );
  }
  else if (isVector() && rhs.isVector())
  {
    if (m_chunk.size() != rhs.m_chunk.size())
      return false;
    ArrayItemPtr::const_iterator  b1 = m_chunk.begin(),
                                  b2 = rhs.chunk().begin(),
                                  e1 = m_chunk.end(),
                                  e2 = rhs.chunk().end();
    
    for( ; b1 != e1 || b2 != e2; b1++, b2++)
    {
      if (*(*b1) == *(*b2))
        continue;
      return false;
    }
    return true;
  }
  return false;
}

void Item::setValue(char value)
{
  m_value = value;
}

ItemPtr Item::addChild()
{
  ItemPtr pRet(new Item());
  m_chunk.push_back(pRet);
  return pRet;
}

void Expression::parse(const std::string& IN str)
{
  if (str.empty()) return;
  m_pAST.reset(new Item());
  const char * pStr = str.c_str();
  std::stack<ItemPtr> stackItemsPtr;
  stackItemsPtr.push(m_pAST);
  TokenType curType = eTokenType_unknow;
  while(*pStr)
  {
    if (isspace(*pStr))
    {
      pStr++;
      continue;
    }
    if (*pStr == '(')
    {
      if (curType != eTokenType_unknow && curType != eTokenType_opening_bracket && curType != eTokenType_operation)
        throw std::invalid_argument(MSG_INVALID_STRING);
      curType = eTokenType_opening_bracket;
      stackItemsPtr.push(stackItemsPtr.top()->addChild());
    }
    else if (*pStr == ')')
    {
      if (curType != eTokenType_closing_bracket && curType != eTokenType_symbol)
        throw std::invalid_argument(MSG_INVALID_STRING);
      curType = eTokenType_closing_bracket;
      stackItemsPtr.pop();
    }
    else if (isalpha(*pStr))
    {
      if (curType != eTokenType_opening_bracket && curType != eTokenType_operation)
        throw std::invalid_argument(MSG_INVALID_STRING);
      curType = eTokenType_symbol;
      stackItemsPtr.top()->setValue(*pStr);
    }
    else if (*pStr == '+')
    {
      if (curType != eTokenType_closing_bracket && curType != eTokenType_symbol)
        throw std::invalid_argument(MSG_INVALID_STRING);
      curType = eTokenType_operation;
      stackItemsPtr.pop();
      if (stackItemsPtr.empty())
        throw std::logic_error(MSG_INVALID_STRING);
      stackItemsPtr.push(stackItemsPtr.top()->addChild());
    }
    else break;
    pStr++;
  }
  if (stackItemsPtr.empty())
  {
    throw std::logic_error("Invalid string");
  }
  if (stackItemsPtr.top() != m_pAST)
  {
    throw std::logic_error("Invalid string");
  }
  stackItemsPtr.pop();
  if (!stackItemsPtr.empty())
  {
    throw std::logic_error("Invalid string");
  }
}
void Expression::getString(std::string& OUT str)
{
  str.clear();
  getSubString(m_pAST, str);
}

void Expression::getSubString(ItemPtr pItem, std::string& OUT str)
{
  if (!pItem.get()) return;
  if (pItem->isValue())
  {
    str += pItem->getValue();
  }
  else
  {
    str += "(";
    ArrayItemPtr chunk = pItem->chunk();
    ArrayItemPtr::size_type i = 0, n = chunk.size() - 1;
    for (ArrayItemPtr::iterator b = chunk.begin(), e = chunk.end(); b != e; ++b, ++i)
    {
      getSubString(*b, str);
      if (i < n)
        str += "+";
    }
    str += ")";
  }
}