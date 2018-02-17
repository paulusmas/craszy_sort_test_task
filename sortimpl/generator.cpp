#include "stdafx.h"

#include "generator.h"
#include <stdlib.h> //srand
#include <time.h>
#include <iostream>
#include <fstream>


const int Generator::MAX_VALUE = 10;

int Generator::getInt() const
{
  return (1 + rand() % (MAX_VALUE - 1));
}

Generator::Generator(int min_value, int max_value)
{
  srand((unsigned int)time(NULL));
  m_min = min_value;
  m_max = max_value;
}

char Generator::getSymbol() const
{
  return (char) ( m_min + rand() % (m_max - m_min) );
}

ItemPtr Generator::generateItem()
{
  MAX_LEVEL = getInt();
  CUR_LEVEL = 0;
  ItemPtr pRet(new Item());
  subGenereteItem(pRet);
  return pRet;
}

ItemPtr Generator::subGenereteItem(ItemPtr pItem)
{
  if ( (rand() % 2 ) == 0  || CUR_LEVEL > MAX_LEVEL)
    pItem->setValue(getSymbol());
  else
  {
    CUR_LEVEL++;
    int n = getInt(); n = (n < 2) ? 2 : n;
    for(int i = 0; i < n; ++i)
    {
      subGenereteItem(pItem->addChild());
    }
  }
  return pItem;
}

bool Generator::run(const char* path, int countStrings)
{
  std::ofstream file(path);
  if (!file.is_open())
    return false;
  for (int i=0; i<countStrings;i++)
  {
    m_pAST.reset(new Item());
    int nItems = getInt();
    m_pAST->chunk().reserve(nItems);
    for(int j = 0; j < nItems; ++j)
    {
      m_pAST->chunk().push_back(generateItem());
    }
    std::string out;
    Expression(m_pAST).getString(out);
    file<<out<<std::endl;
    std::cout << "Generate: " << i + 1 << std::endl;
  }
  file.close();
  return true;
}

bool Generator::run()
{
  return true;
}

bool API_EXPORT GenerateFile(const std::string& filename, int N)
{
  Generator generator('A', 'Z');  
  std::cout << "Please wait. Process generate has been started...\n"; 
  return generator.run(filename.c_str(), N);
}

PathHelper::PathHelper(const std::string& str)
{
  m_path = str;
  int l = str.length() - 1;
  const char * pStr = str.c_str();
  pStr += l;
  int i = l;
  while(*pStr != '.' || i < 0 )
  {
    pStr--;
    i--;
  }
  if (l - i <= 0 ) return;
  int size = l - i;
  m_ext.resize(size);
  memcpy(&m_ext[0], str.c_str() + i + 1, size);
  
  while(*pStr != '\\' && i >= 0 )
  {
    pStr--;
    i--;
  }
  size = l - i - m_ext.length() - 1;
  m_filename.resize(size);
  memcpy(&m_filename[0], str.c_str() + i + 1, size);
  
  pStr -= size;
  
  size = l - m_filename.size() - m_ext.size();
  m_path.resize(size);
  if (!size) return;
  memcpy(&m_path[0], str.c_str(), size);
}