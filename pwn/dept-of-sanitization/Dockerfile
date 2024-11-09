FROM ubuntu:22.04

RUN apt-get update && apt-get install -y build-essential clang socat

WORKDIR /chal
COPY . .

RUN CC=clang CXX=clang++ make SAN_DEBUG=1 FLAG=csawctf{g0tta_luv_p1zza_r@t}

RUN useradd -ms /bin/sh ctf

RUN chown -R root:ctf /chal \
     && chmod 750 /chal \
     && chmod 550 /chal/dept_of_sanitization

USER ctf
EXPOSE 5454
CMD ["socat", "-T60", "TCP-LISTEN:5454,reuseaddr,fork", "EXEC:'env LD_PRELOAD=/chal/libsan.so /chal/dept_of_sanitization'"]
