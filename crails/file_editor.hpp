#pragma once
#include <string>

class CrailsFileEditor
{
protected:
  std::string  path, symbol;
  std::string  contents;
  unsigned int position;
  std::string  prefix_pattern;
public:
  CrailsFileEditor(const std::string& path);

  bool load_file();
  bool use_symbol(const std::string& symbol);
  void insert(const std::string& text);
  void add_include(const std::string& path);
  void save_file();
};

class CppFileEditor : public CrailsFileEditor
{
public:
  CppFileEditor(const std::string& path) : CrailsFileEditor(path)
  {
    prefix_pattern = "//";
  }

  void add_include(const std::string& path) { contents.insert(0, "#include \"" + path + "\"\n"); }
};
