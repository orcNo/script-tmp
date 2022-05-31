#!/bin/bash

if [[ $# -lt 1 ]]; then
	echo "Usage: $0 <password>"
	exit
fi

src_path="/home/longq/work/sds/script/"
dst_path="/home/web/certs"
back_path="/home/backup"
pw_path="var/lib/nginx/ssl_passwords.txt"

ts=$(data +%s)

mkdir -p "${back_path}/${ts}"
mkdir -p "${src_path}"
mkdir -p "${dst_path}"

#pw1=$(( RANDOM % 10000 ))
#pw2=$(( RANDOM % 10000 ))
#pw="${pw1}${pw2}"
pw=$1

#backup
cp "${dst_path}/*.pem" "${back_path}/${ts}/"
cp "${pw_path}" ${back_path}/${ts}/"

#override new file
cp "${src_path}/root/root.pem" "${dst_path}/ca.crt"
cp "${src_path}/tunnel/tunel-gateway-tunnel.key" "${dst_path}/enc_key.pem"
cp "${src_path}/tunnel/tunel-gateway-tunnel.pem" "${dst_path}/enc_cert.pem"
cp "${src_path}/tunnel/tunel-gateway-sign.key" "${dst_path}/enc_key.pem"
cp "${src_path}/tunnel/tunel-gateway-sign.pem" "${dst_path}/enc_cert.pem"
cp "${src_path}/crl/crl.crl" "${dst_path}/test.crl"
echo "${pw}" > ${pw_path}

systemd restart ngnixgmht
