#pragma once
#include <filesystem>
#include <string>
#include <string_view>
#include <set>

class CrailsFileEditor
{
protected:
  std::string  path, symbol;
  std::string  contents;
  unsigned int position;
  std::string  prefix_pattern;
public:
  CrailsFileEditor(const std::filesystem::path&);

  bool load_file();
  bool use_symbol(const std::string& symbol, bool whole_line = true);
  void insert(const std::string& text, int offset = 0);
  void append(const std::string& text) { contents += text; }
  void prepend(const std::string& text) { contents.insert(0, text); }
  void save_file();
  void set_prefix_pattern(const std::string& prefix) { prefix_pattern = prefix; }
  std::size_t find(const std::string& str, std::size_t pos = 0) { return contents.find(str, pos); }
  void erase(std::size_t begin, std::size_t end) { contents.erase(begin, end); }
  std::string_view view() const { return std::string_view(contents); }
};

class CppFileEditor : public CrailsFileEditor
{
public:
  CppFileEditor(const std::string& path, const std::string& prefix = "//") : CrailsFileEditor(path)
  {
    prefix_pattern = prefix;
  }

  void add_include(const std::string& path) { prepend("#include \"" + path + "\"\n"); }
};

class MainCppEditor : public CppFileEditor
{
public:
  MainCppEditor(const std::string& path = "main.cpp") : CppFileEditor(path, "")
  {
  }

  void add_to_main_function(const std::string& value)
  {
    if (find(value) < 0)
    {
      use_symbol("int\\s+main\\s*\\([^)]+\\)\\s*\\{");
      insert("  " + value);
    }
  }
};

class RequestPipeCppEditor : public CppFileEditor
{
public:
  RequestPipeCppEditor() : CppFileEditor("config/request_pipe.cpp", "")
  {
  }

  void add_to_request_pipe(const std::string& value)
  {
    use_symbol("void\\s+(Crails::)?\\s*Server\\s*::\\s*initialize_request_pipe\\s*\\(\\s*(void)?\\s*\\)\\s*\\{");
    insert("  " + value);
  }
};

class ProjectConfiguration;
class CMakeFileEditor : public CrailsFileEditor
{
public:
  CMakeFileEditor(const ProjectConfiguration&);

  void update_plugins(const std::string& line);
  void add_dependency(const std::string& name, const std::string& category = "dependencies");
  void add_definitions(const std::set<std::string>&);
  void remove_definitions(const std::set<std::string>&);
  void add_task(const std::string& name);
  void add_module(const std::string& name);
  void remove_module(const std::string& name);

private:
  const ProjectConfiguration& configuration;
};
