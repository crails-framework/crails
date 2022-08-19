#pragma once
#include <boost/filesystem.hpp>
#include <regex>
#include <functional>

class FileCollector
{
public:
  typedef std::function<void(const boost::filesystem::path&)> Callback;

  FileCollector(const std::string& input, const std::string& pattern);

  std::string relative_path_for(const boost::filesystem::path&) const;
  std::string name_for(const boost::filesystem::path&) const;
  void        collect_files(Callback callback) const { collect_files(boost::filesystem::canonical(root_path), callback); }
private:
  void        collect_files(const boost::filesystem::path& directory, Callback callback) const;

  const boost::filesystem::path root_path;
  unsigned int                  root_path_length;
  const std::regex              pattern;
};
