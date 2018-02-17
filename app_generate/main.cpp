#include <iostream>
#include <assert.h>

#include "../common/include/exports.h"

using namespace std;

int main(int argc, char *argv[])
{
  std::string file;
  int N = 1;
  if (argc < 3)
  {
    file = PathHelper(std::string(argv[0])).path() + std::string(DEFAULT_NAME_FILE);    
    do{
      std::cout << "Input number of lines N: ";
      std::cin >> N;
      if (N <= 0) std::cout <<"\n N must be greater 0. Try again...\n";
    }while (N <= 0);
  }
  else
  {
     N = std::atoi(argv[1]);
     file = argv[2];
  }
  if (GenerateFile(file, N))
    std::cout << "File " << file.c_str() << " has been created.\n";
  else std::cout << "File " << file.c_str() << " can't created.\n";
  system("pause");
  return 0;
}