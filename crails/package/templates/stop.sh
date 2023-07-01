std::string @application_name;
// END LINKING
#!/bin/sh -ex

if [ -z "$APPLICATION_NAME" ] ; then export APPLICATION_NAME="<%= application_name %>" ; fi
if [ -z "$PID_FILE" ]         ; then export PID_FILE="/tmp/$APPLICATION_NAME.pid" ; fi

if [ -f "$PID_FILE" ] ; then
  pid=`cat "$PID_FILE"`
  kill -INT $pid
  sleep 1
  while [ $? -ne 0 ] ; do
    kill -TERM $pid
  done
  rm "$PID_FILE"
else
  exit -1
fi
