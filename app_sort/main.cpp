#include <iostream>
#include <fstream>
#include <assert.h>
#include "../common/include/exports.h"

#include <Windows.h>

#define _SIMPLE_TEST_

void compare(const std::string& s1, const std::string& s2)
{
  bool ret = s1 == s2;
  if (ret)
    std::cout << "PASS: "<<s2.c_str()<<std::endl;
  else 
    std::cout << "FAIL: " << s1.c_str() << "!=" << s2.c_str()<<std::endl;
  assert(ret);
}

void simpleTest()
{
#if defined(_SIMPLE_TEST_) && _DEBUG
  std::cout<<"          SIMPLE TEST          "<<std::endl;
  std::string out;
  A("(X+(V+M+A)+C+(M+L+(C+C)+C))", out);
  compare(out, "((A+M+V)+C+(C+(C+C)+L+M)+X)");
  A("(((((Z+Q) +A )+C )+A )+Z + C + (A+A+C + ( ( ( (Z+Q) +A )+C )+A )) + ( ( ( (B+A) +A )+C )+A ) + (A+B) + (A+A+A))", out);
  compare(out, "((A+A+A)+(A+((A+(A+B))+C))+(A+((A+(Q+Z))+C))+(A+A+(A+((A+(Q+Z))+C))+C)+(A+B)+C+Z)");
  A("(D+B+C)", out);  
  compare(out, "(B+C+D)");
  A("((B+A)+A)", out);
  compare(out, "(A+(A+B))");
  A("(Z+(Q+A+C) + (C+A) + B)", out);
  compare(out, "((A+C)+(A+C+Q)+B+Z)"); 
  A("(B+(D+(C+B+A)))", out);
  compare(out, "(((A+B+C)+D)+B)");
  std::cout<<"==============================="<<std::endl;
#endif
}



int main(int argc, char *argv[])
{
  simpleTest();
  std::string file;
  if (argc < 2)
  {
    file = PathHelper(std::string(argv[0])).path() + std::string(DEFAULT_NAME_FILE);
    if (!GenerateFile(file, 5))
      return 0;
  }
  else file = argv[1];
  std::string str;
  std::ifstream generateFile(file.c_str());
  if (!generateFile.is_open())
  {
    std::cout<< "File" <<file.c_str()<< " is not found.\n";
    return 0;
  }
  PathHelper helper(file);
  std::ofstream resultFile(std::string(helper.path() + helper.filename() + "_sorted" + "." + helper.ext()).c_str());
  while (!generateFile.eof())
  {
    generateFile >> str; 
    std::string out;
    A(str, out);
    resultFile << out<<'\n';
    std::cout<<"INPUT: "<<str<<std::endl;
    std::cout<<"SORTED: "<<out<<std::endl<<std::endl;
  }
  generateFile.close();
  resultFile.close();
  system("pause");
  return 0;
}