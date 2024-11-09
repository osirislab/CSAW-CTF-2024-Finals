FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive \
  apt -y update && apt -y upgrade && \
  apt -y install qemu-user qemu-user-static socat

RUN useradd -ms /bin/sh chal
WORKDIR /home/chal

COPY ./riscy_voting .
COPY ./flag.txt /home/chal/flag.txt

RUN chown -R root:chal /home/chal && \
    chmod 750 /home/chal && \
    chmod 550 /home/chal/riscy_voting && \
    chown root:chal /home/chal/flag.txt && \
    chmod 444 /home/chal/flag.txt

EXPOSE 7702

CMD ["socat","-T6000", "TCP-LISTEN:7702,reuseaddr,fork,su=chal","EXEC:/usr/bin/qemu-riscv64-static riscy_voting"]
