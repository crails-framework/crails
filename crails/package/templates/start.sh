std::string @bin_directory;
std::string @lib_directory;
std::string @share_directory;
std::string @pidfile;
// END LINKING
#!/bin/sh -ex

current_dir=`dirname $0`
cd "<%= share_directory %>"

if [ -f "./environment" ] ; then
  . "./environment"
fi

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:<%= lib_directory %>"
if [ -z "$HOSTNAME" ] ; then export HOSTNAME="0.0.0.0" ; fi
if [ -z "$PORT" ]     ; then export PORT="3001" ; fi

exec "$current_dir/server" \
  --hostname "$HOSTNAME" \
  --port     "$PORT" \
  --pidfile  "<%= pidfile %>"
