#pragma once
#include "defs.h"
#include <string>

void API_EXPORT A(const std::string& in, std::string& out);
bool API_EXPORT GenerateFile(const std::string& filename, int N); 

class API_EXPORT PathHelper
{
public:
  PathHelper(const std::string& str);
  const std::string& path() const { return m_path; };
  const std::string& filename() const { return m_filename; }
  const std::string& ext() const { return m_ext; }

private:
  std::string m_path;
  std::string m_filename;
  std::string m_ext;
};


