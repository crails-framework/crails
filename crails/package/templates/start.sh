unsigned short @application_port = 80;
std::string @application_host = "0.0.0.0";
std::string @application_name;
std::string @bin_directory;
std::string @lib_directory;
std::string @share_directory;
std::string @runtime_path = "/var/lib/" + application_name;
// END LINKING
#!/bin/sh -ex

export APPLICATION_BIN="$(cd "$( dirname "$0" )" && pwd)"

if [ -z "$VAR_DIRECTORY" ]        ; then export VAR_DIRECTORY="<%= runtime_path %>" ; fi
if [ -z "$APPLICATION_HOSTNAME" ] ; then export APPLICATION_HOSTNAME="<%= application_host %>" ; fi
if [ -z "$APPLICATION_PORT" ]     ; then export APPLICATION_PORT="<%= application_port %>" ; fi
if [ -z "$APPLICATION_NAME" ]     ; then export APPLICATION_NAME="<%= application_name %>" ; fi
if [ -z "$PID_FILE" ]             ; then export PID_FILE="/tmp/$APPLICATION_NAME.pid" ; fi

cd "$VAR_DIRECTORY"

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:<%= lib_directory %>:<%= bin_directory %>"
export PUBLIC_PATH="<%= share_directory %>/public"

exec "<%= bin_directory %>/server" \
  --hostname "$APPLICATION_HOSTNAME" \
  --port     "$APPLICATION_PORT" \
  --pidfile  "$PID_FILE" \
  --log      "/var/log/$APPLICATION_NAME/event.log"
