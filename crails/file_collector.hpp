#pragma once
#include <filesystem>
#include <regex>
#include <functional>

class FileCollector
{
public:
  typedef std::function<void(const std::filesystem::path&)> Callback;

  FileCollector(const std::string& input, const std::string& pattern);

  std::string relative_path_for(const std::filesystem::path&) const;
  std::string name_for(const std::filesystem::path&) const;
  void        collect_files(Callback callback) const { collect_files(std::filesystem::canonical(root_path), callback); }
private:
  void        collect_files(const std::filesystem::path& directory, Callback callback) const;

  const std::filesystem::path root_path;
  unsigned int                root_path_length;
  const std::regex            pattern;
};
