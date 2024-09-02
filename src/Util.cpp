#include <string>

std::string uid_escape(std::string s)
{
  const char* esc = "\\>:{},";
  std::size_t pos = 0;
  pos = s.find_first_of(esc, pos);
  while (pos != std::string::npos)
  {
    s.insert(pos, "\\");
    pos = s.find_first_of(esc, pos+2);
  }
  return s;
}
