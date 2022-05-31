#!/bin/bash

cert_dgst_dir="/etc/dmp/check-cert/"
dgst_script_path="/home/longq/work/sds/script/genDgst.sh"

tmp_file="tmp.hash"
cur_file="cur.hash"
old_file="old.hash"

mkdir -p ${cert_dgsts_dir}

#
. ${dgst_script_path} > "${cert_dgst_dir}/${tmp_file}"
new_hash=$(cat "${cert_dgst_dir}/${tmp_file}")
cur_hash=$(cat "${cert_dgst_dir}/${cur_file}")

echo "${cur_hash}" > "${cert_dgst_dir}/${old_file}" 
echo "${new_hash}" > "${cert_dgst_dir}/${cur_file}" 

if [[ "${new_hash}" -nq "${cur_hash}" ]]; then
	echo "files changed."
	exit -1
else
	echo "same."
	exit 0
fi
