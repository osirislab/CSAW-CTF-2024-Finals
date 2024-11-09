FROM python:3.9.13

ENV PORT=31337
ENV HTTP_PORT=8545
ENV PYTHONPATH /usr/lib/python

RUN true \
    && mkdir /var/log/ctf /startup \
    && apt update \
    && apt install -y xinetd tini \
    && rm -rf /var/cache/apt/archives \
    && useradd -m ctf \
    && true

COPY files/00-create-xinetd-service /startup
COPY files/98-start-gunicorn /startup
COPY files/99-start-xinetd /startup
COPY eth_sandbox /usr/lib/python/eth_sandbox

COPY files/handler.sh /home/ctf/handler.sh
COPY files/entrypoint.sh /entrypoint.sh

COPY files/requirements.txt /root

RUN python3 -m pip install -r /root/requirements.txt 

RUN true \
    && curl -L https://foundry.paradigm.xyz | bash \
    && bash -c "source /root/.bashrc && foundryup" \
    && chmod 755 -R /root \
    && true

COPY deploy/ /home/ctf/
COPY contracts /tmp/contracts

RUN true \
    && cd /tmp \
    && /root/.foundry/bin/forge build --evm-version cancun --via-ir --out /home/ctf/compiled \
    && rm -rf /tmp/contracts \
    && true

ENTRYPOINT ["tini", "-g", "--"]
CMD ["/entrypoint.sh"]