#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>

template <class T>
struct SmartPtr
{
  typedef std::tr1::shared_ptr<T> ptr;
};

class Item;
typedef SmartPtr<Item>::ptr ItemPtr;
typedef std::vector<ItemPtr> ArrayItemPtr;


class Item
{
public:
  Item() { m_value = 0; }
  
  char getValue() const { return m_value; }
  void setValue(char value);
  
  const ArrayItemPtr& chunk() const;
  ArrayItemPtr& chunk();
  
  bool isValue() const;
  bool isVector() const;
  
  ItemPtr addChild();
  
  template<class _Pr>
  struct SortFunctor 
  { 
    void operator()(ItemPtr pItem) { pItem->subSort<_Pr>(); }
  };
  template<class _Pr>
  void subSort()
  {
    if (!isVector())
      return;
    std::for_each(chunk().begin(), chunk().end(), SortFunctor<_Pr>());
    std::sort(chunk().begin(), chunk().end(), _Pr());
  }

  unsigned int logicalLength() const;
  
  bool operator < (const Item& rhs) const
  {
    return subLess(rhs);
  }

  bool operator > (const Item& rhs) const
  {
    return subGreater(rhs);
  }
  
  bool operator == (const Item& rhs) const
  {
    return subEqual(rhs);
  }

  bool operator != (const Item& rhs) const
  {
    return !subEqual(rhs);
  }
  
protected:
  bool subLess(const Item& rhs) const;
  bool subGreater(const Item& rhs) const;
  bool subEqual(const Item& rhs) const;
  unsigned int subLogicalLength(const Item& rhs) const;

private:
  char m_value;
  ArrayItemPtr m_chunk;
};

class ItemLessPredicate 
: std::binary_function<ItemPtr, ItemPtr, bool>
{
public:
  result_type operator()(const first_argument_type& l
    , const second_argument_type& r) const
  {
    return *l < *r;
  }
};

class ItemGreaterPredicate 
: std::binary_function<ItemPtr, ItemPtr, bool>
{
public:
  result_type operator()(const first_argument_type& l
    , const second_argument_type& r) const
  {
    return *l > *r;
  }
};

class Expression
{
public:
  enum TokenType
  {
    eTokenType_unknow,
    eTokenType_operation,
    eTokenType_symbol,
    eTokenType_opening_bracket,
    eTokenType_closing_bracket
  };
public:
  Expression() {}
  Expression(const ItemPtr& pAST) 
  : m_pAST(pAST) {}
  virtual ~Expression() {}
  void parse(const std::string& IN str);
  
  template<class _Pr>
  void sort()
  {
    if (m_pAST)
        m_pAST->subSort<_Pr>();
  }
  void getString(std::string& OUT str);
  void generate();
protected:
  void getSubString(ItemPtr pItem, std::string& OUT str);
private:
  ItemPtr m_pAST;
};