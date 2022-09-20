std::string @pidfile;
// END LINKING
#!/bin/sh -ex

pidfile="<%= pidfile %>"

if [ -f "$pidfile" ] ; then
  pid=`cat "$pidfile"`
  kill -INT $pid
  sleep 1
  while [ $? -ne 0 ] ; do
    kill -TERM $pid
  done
  rm "$pidfile"
else
  exit -1
fi
