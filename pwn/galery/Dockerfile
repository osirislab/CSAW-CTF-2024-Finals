FROM ubuntu:22.04

RUN apt-get update && apt-get install -y socat

RUN useradd -m galery

WORKDIR /home/galery

COPY ./galery .
COPY ./flag.txt .

RUN chown -R galery:galery /home/galery && \
    chmod 750 /home/galery && \
    chmod 550 galery && \
    chown galery:galery flag.txt && \
    chmod 440 flag.txt

EXPOSE 9991

USER galery

CMD ["socat", "-T60", "TCP-LISTEN:9991,reuseaddr,fork","EXEC:/home/galery/galery,stderr"]