FROM ubuntu:22.04 AS builder
RUN DEBIAN_FRONTEND=noninteractive \
  apt -y update && apt -y upgrade && \
  apt -y install wget autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build

RUN wget https://github.com/riscv-collab/riscv-gnu-toolchain/releases/download/2024.09.03/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.09.03-nightly.tar.gz

RUN tar xf riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.09.03-nightly.tar.gz
ENV PATH="$PATH:/riscv/bin"
RUN mkdir -p /src
RUN mkdir -p /build
COPY ./src/riscy_voting.c /src/riscy_voting.c
WORKDIR /src
RUN /riscv/bin/riscv64-unknown-linux-gnu-gcc -w -static riscy_voting.c -o ../build/riscy_voting
RUN /riscv/bin/riscv64-unknown-linux-gnu-strip --strip-all ../build/riscy_voting

FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive \
  apt -y update && apt -y upgrade && \
  apt -y install qemu-user qemu-user-static socat && \
  apt -y install net-tools gdbserver gdb-multiarch

RUN useradd -ms /bin/sh chal
WORKDIR /home/chal

COPY --from=builder /build/riscy_voting .
COPY ./flag.txt /home/chal/flag.txt

RUN chown -R root:chal /home/chal && \
    chmod 750 /home/chal && \
    chmod 550 /home/chal/riscy_voting && \
    chown root:chal /home/chal/flag.txt && \
    chmod 444 /home/chal/flag.txt

EXPOSE 7704

CMD ["socat","-T6000", "TCP-LISTEN:7704,reuseaddr,fork,su=chal","EXEC:/usr/bin/qemu-riscv64-static riscy_voting"]
