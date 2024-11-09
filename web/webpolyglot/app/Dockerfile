FROM python:3.9-slim-buster

RUN useradd -r ctf

WORKDIR /app

COPY requirements.txt ./
RUN pip3 install -r requirements.txt

COPY ./ ./
COPY .env .env

RUN chown -R ctf /app

USER ctf

EXPOSE 4747

CMD ["sh", "-c", "export $(grep -v '^#' .env | xargs) && gunicorn -b 0.0.0.0:4747 -w 1 app:app"]
