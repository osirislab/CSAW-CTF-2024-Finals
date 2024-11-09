#!/bin/bash
set -e

PORT="$1"
HTTP_PORT="$2"
SHARED_SECRET="$RANDOM$RANDOM$RANDOM$RANDOM$RANDOM$RANDOM"

FLAG="flag{not_the_flag}"
PUBLIC_IP=127.0.0.1

echo "[+] running challenge"
exec docker run \
    -e "PORT=$PORT" \
    -e "HTTP_PORT=$HTTP_PORT" \
    -e "FLAG=$FLAG" \
    -e "PUBLIC_IP=$PUBLIC_IP" \
    -e "SHARED_SECRET=$SHARED_SECRET" \
    -p "$PORT:$PORT" \
    -p "$HTTP_PORT:$HTTP_PORT" \
    challenge:latest