#pragma once
#include "file_editor.hpp"

class CrailsPackageListEditor : public CrailsFileEditor
{
public:
  CrailsPackageListEditor() : CrailsFileEditor("CMakeLists.txt")
  {
    prefix_pattern = "pkg_check_modules\\s*\\(";
  }

  bool update_module_list(const std::list<std::string>& modules, const std::string& version)
  {
    if (use_symbol("CRAILS REQUIRED"))
    {
      for (unsigned int i = position ; contents[i] != ')' && i < contents.length() ; ++i);
      contents = contents.substr(0, position) + contents.substr(i);
      for (const std::string& module_ : modules)
        insert(" " + module_ + ">=" + version);
      save_file();
      return true;
    }
    return false;
  }
};
