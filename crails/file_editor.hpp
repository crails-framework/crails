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
  CppFileEditor(const std::string& path, const std::string& prefix = "//") : CrailsFileEditor(path)
  {
    prefix_pattern = prefix;
  }

  void add_include(const std::string& path) { contents.insert(0, "#include \"" + path + "\"\n"); }
};

class ProjectConfiguration;
class CMakeFileEditor : public CrailsFileEditor
{
public:
  CMakeFileEditor(ProjectConfiguration&);

  std::string module_config_line() const;
  void update_modules();
  void add_dependency(const std::string& name);
  void add_task(const std::string& name);

private:
  ProjectConfiguration& configuration;
};
