FROM python:3.10-slim

RUN apt-get update \
     && apt-get install -y socat python3 \
     && useradd -ms /bin/sh ctf

WORKDIR /chal

COPY ./repeat ./flag.txt ./

RUN chown -R root:ctf /chal \
     && chmod 750 /chal \
     && chmod 550 /chal/repeat \
     && chown root:ctf /chal/flag.txt \
     && chmod 440 /chal/flag.txt

EXPOSE 5454

USER ctf

CMD ["socat", "-T60", "TCP-LISTEN:5454,reuseaddr,fork", "EXEC:/chal/repeat"]
