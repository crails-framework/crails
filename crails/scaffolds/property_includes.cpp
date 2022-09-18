#include "helpers.hpp"

using namespace std;

string includes_for_properties(map<string, string>& properties)
{
  string result;
  map<string, string> includes{
    {"Crails::Password","crails/signin/password.hpp"}
  };

  for (auto it = properties.begin() ; it != properties.end() ; ++it)
  {
    auto include = includes.find(it->second);

    if (include != includes.end())
    {
      result += "#include <" + include->second + ">\n";
      includes.erase(include);
    }
  }
  if (result.length() > 0)
    return '\n' + result;
  return result;
}
