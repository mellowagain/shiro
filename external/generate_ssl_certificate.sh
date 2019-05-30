#!/bin/bash

read -p "What's the name of your server?" server_name
server_name=${server_name=Shiro}

read -p "What country are you based in? (i.e DE = Germany)" country_name
country_name=${country_name=GB}

args="/CN=*.ppy.sh/O=$server_name/C=$country_name"
openssl req -subj $args -new -newkey rsa:4096 -sha256 -days 36500 -nodes -x509 -keyout key.pem -out cert.pem
openssl x509 -outform der -in cert.pem -out cert.crt
