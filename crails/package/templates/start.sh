std::string @application_name;
std::string @bin_directory;
std::string @lib_directory;
std::string @share_directory;
std::string @runtime_path = "/var/" + application_name;
std::string @pidfile;
std::string environment_file = share_directory + "/environment";
// END LINKING
#!/bin/sh -ex

if [ -f "<%= environment_file %>" ] ; then
  . "<%= environment_file %>"
fi

cd "<%= runtime_path %>"

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:<%= lib_directory %>:<%= bin_directory %>"
export PUBLIC_PATH="<%= share_directory %>/public"
if [ -z "$HOSTNAME" ] ; then export HOSTNAME="0.0.0.0" ; fi
if [ -z "$PORT" ]     ; then export PORT="3001" ; fi

exec "<%= bin_directory %>/server" \
  --hostname "$HOSTNAME" \
  --port     "$PORT" \
  --pidfile  "<%= pidfile %>" \
  --log      "/var/log/<%= application_name %>/`date +%y-%m-%d_%Hh%Mm%S``_crails.log"
