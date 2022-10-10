unsigned short @application_port = 3001;
std::string @application_host = "0.0.0.0";
std::string @application_name;
std::string @bin_directory;
std::string @lib_directory;
std::string @share_directory;
std::string @runtime_path = "/var/" + application_name;
std::string @pidfile;
std::string environment_file = share_directory + "/environment";
// END LINKING
#!/bin/sh -ex

export APPLICATION_BIN="$(cd "$( dirname "$0" )" && pwd)"
export APPLICATION_ENV="<%= environment_file %>"

if [ -f "$APPLICATION_ENV" ] ; then
  . "$APPLICATION_ENV"
fi

cd "<%= runtime_path %>"

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:<%= lib_directory %>:<%= bin_directory %>"
export PUBLIC_PATH="<%= share_directory %>/public"
if [ -z "$APPLICATION_HOSTNAME" ] ; then export APPLICATION_HOSTNAME="<%= application_host %>" ; fi
if [ -z "$APPLICATION_PORT" ]     ; then export APPLICATION_PORT="<%= application_port %>" ; fi

exec "<%= bin_directory %>/server" \
  --hostname "$APPLICATION_HOSTNAME" \
  --port     "$APPLICATION_PORT" \
  --pidfile  "<%= pidfile %>" \
  --log      "/var/log/<%= application_name %>/event.log"
