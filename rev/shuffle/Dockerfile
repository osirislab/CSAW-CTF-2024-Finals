FROM ubuntu:22.04

RUN apt-get update && apt-get install -y socat g++

RUN useradd -ms /bin/sh ctf
WORKDIR /home/ctf

COPY ./shuffle ./
COPY ./flag.txt ./

RUN  chown -R root:root /home/ctf && \
     chmod 755 /home/ctf && \
     chmod 555 /home/ctf/shuffle && \
     chmod 444 ./flag.txt

EXPOSE 9999

USER ctf

CMD ["socat", "-T60", "TCP-LISTEN:9999,reuseaddr,fork","EXEC:/home/ctf/shuffle,pty,stderr"]
