#!/bin/bash

host=$1
user=$2
passwd=$3
dir=$4
file=$5

ftp -niv << EOF
open $host
user $user $passwd
bin
cd $dir
get $file
bye
EOF

exit 0
