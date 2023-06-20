#include "file_collector.hpp"
#include <iostream>

using namespace std;

FileCollector::FileCollector(const string& input, const string& pattern) : root_path(input), pattern(pattern)
{
  root_path_length = filesystem::canonical(root_path).string().length();
}

void FileCollector::collect_files(const filesystem::path& directory, Callback callback) const
{
  if (filesystem::is_directory(directory))
  {
    filesystem::recursive_directory_iterator dir(directory);

    for (auto& entry : dir)
    {
      string filepath = filesystem::canonical(entry.path()).string();
      string filename = entry.path().filename().string();
      auto   match    = sregex_iterator(filename.begin(), filename.end(), pattern);

      if (filesystem::is_directory(entry))
      {
        collect_files(entry.path(), callback);
        continue ;
      }
      else if (match == sregex_iterator())
        continue ;
      callback(entry.path());
    }
  }
}

string FileCollector::relative_path_for(const filesystem::path& path) const
{
  return filesystem::canonical(path).string().substr(root_path_length + 1);
}

string FileCollector::name_for(const filesystem::path& path) const
{
  string relative_path = relative_path_for(path);

  if (path.has_stem())
    return relative_path.substr(0, relative_path.length() - path.extension().string().length());
  return relative_path;
}
