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
  bool use_symbol(const std::string& symbol, bool whole_line = true);
  void insert(const std::string& text);
  void save_file();
  void set_prefix_pattern(const std::string& prefix) { prefix_pattern = prefix; }
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

  std::string plugins_config_line() const;
  void update_plugins();
  void add_dependency(const std::string& name);
  void add_task(const std::string& name);
  void add_module(const std::string& name);
  void remove_module(const std::string& name);

private:
  ProjectConfiguration& configuration;
};
