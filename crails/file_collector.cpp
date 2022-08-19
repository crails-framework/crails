#include "file_collector.hpp"
#include <iostream>

using namespace std;

FileCollector::FileCollector(const string& input, const string& pattern) : root_path(input), pattern(pattern)
{
  root_path_length = root_path.string().length();
}

void FileCollector::collect_files(const boost::filesystem::path& directory, Callback callback) const
{
  boost::filesystem::recursive_directory_iterator dir(directory);

  for (auto& entry : dir)
  {
    string filepath = boost::filesystem::canonical(entry.path()).string();
    string filename = entry.path().filename().string();
    auto   match    = sregex_iterator(filename.begin(), filename.end(), pattern);

    if (boost::filesystem::is_directory(entry))
    {
      collect_files(entry.path(), callback);
      continue ;
    }
    else if (match == sregex_iterator())
      continue ;
    callback(entry.path());
  }
}

string FileCollector::relative_path_for(const boost::filesystem::path& path) const
{
  return boost::filesystem::canonical(path).string().substr(root_path_length + 1);
}

string FileCollector::name_for(const boost::filesystem::path& path) const
{
  string relative_path = relative_path_for(path);

  if (path.has_stem())
    return relative_path.substr(0, relative_path.length() - path.extension().string().length());
  return relative_path;
}
