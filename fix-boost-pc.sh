#!/bin/sh

INSTALL_ROOT="/usr/local"
list="icuuci icuuc icui18n"
boost_list="any \
     function \
     bind \
     type_index \
     lexical_cast \
     numeric_conversion \
     dynamic_bitset \
     range \
     array \
     iterator \
     conversion \
     fusion \
     function_types \
     tuple \
     typeof \
     optional \
     detail \
     regex \
     concept_check \
     integer \
     program_options_types \
     random_types \
     mpl \
     utility \
     io \
     preprocessor \
     smart_ptr \
     move \
     core \
     type_traits \
     static_assert \
     system \
     throw_exception \
     variant2 \
     assert \
     mp11 \
     winap \
     config \
     predef \
     asio \
     align \
     date_time \
     algorithm \
     exception \
     unordered \
     tokenizer_types \
     endian \
     intrusivei \
     logic"

for item in $boost_list ; do
  list="$list boost_$item"
done

for pc_file in `ls "$INSTALL_ROOT/lib/pkgconfig/"*.pc` ; do
  echo "boost-pc monkeypatch: checking $pc_file"
  for library in $list ; do
    awk '{ if ($0 ~ /^Libs:/) { gsub(" -l'$library'", ""); print $0 } else { print $0 } }' \
      "$pc_file" > tmpfile
    mv tmpfile "$pc_file"
  done
done
