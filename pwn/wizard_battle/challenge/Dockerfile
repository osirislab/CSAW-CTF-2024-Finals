FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive \
  apt -y update && apt -y upgrade && \
  apt -y install qemu-user qemu-user-static socat

RUN useradd -ms /bin/sh chal
WORKDIR /home/chal

COPY ./wizard_battle /home/chal/wizard_battle
COPY ./flag.txt /home/chal/flag.txt
COPY ./optout /home/chal/optout

RUN chown -R root:chal /home/chal && \
    chmod 750 /home/chal && \
    chmod 550 /home/chal/wizard_battle && \
    chown root:chal /home/chal/flag.txt && \
    chmod 444 /home/chal/flag.txt && \
    chmod 660 /home/chal/optout

EXPOSE 7701

CMD ["socat","-T6000", "TCP-LISTEN:7701,reuseaddr,fork,su=chal","EXEC:/usr/bin/qemu-riscv64-static wizard_battle"]
