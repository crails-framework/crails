#pragma once
#include <boost/filesystem.hpp>

class WithPath
{
  const boost::filesystem::path old_path;
public:
  WithPath(boost::filesystem::path new_path) : old_path(boost::filesystem::current_path())
  {
    boost::filesystem::create_directories(new_path);
    boost::filesystem::current_path(new_path);
  }

  ~WithPath()
  {
    boost::filesystem::current_path(old_path);
  }
};
