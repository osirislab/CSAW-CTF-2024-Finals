#!/bin/bash
set -e

cd challenge
docker build -t challenge:latest .

PORT="$1"
HTTP_PORT="$2"
SHARED_SECRET="$RANDOM$RANDOM$RANDOM$RANDOM$RANDOM$RANDOM"

FLAG="csawctf{br0k3n_c0mp1l3r_for_th3_w1n}"
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