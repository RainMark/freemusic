#!/bin/bash

host=$1
user=$2
passwd=$3
src_dir=$4
file=$5
dst_dir=$6

cd $dst_dir
ftp -niv << EOF
open $host
user $user $passwd
bin
cd $src_dir
get $file
bye
EOF

exit 0
