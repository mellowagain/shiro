#!/usr/bin/env bash

# Asks for server name
read -p "What's the name of your server? \n" server_name
# Checks if input is blank, if so, set server name to "Shiro"
server_name=${server_name=Shiro}

# Asks for country code
read -p "What is the country code of the country are you based in? (i.e DE = Germany) \n" country_name
# Again, checks if input is blank, if not, sets the country code to GB.
country_name=${country_name=GB}

# Creates the necessary args needed
args="/CN=*.ppy.sh/O=$server_name/C=$country_name"
# Creates the SSL certificate using the args we created
openssl req -subj $args -new -newkey rsa:4096 -sha256 -days 36500 -nodes -x509 -keyout key.pem -out cert.pem
# Convert it to .crt
openssl x509 -outform der -in cert.pem -out cert.crt
printf "Your SSL Certificates have been generated, it's a good idea to move them to a seperate directory now."
