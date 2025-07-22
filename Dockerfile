FROM ubuntu:latest

WORKDIR /app

RUN apt update && apt install -y gcc

COPY . .

RUN chmod +x ./docker-utils/makeexec.sh

RUN ./docker-utils/makeexec.sh

EXPOSE 8096

CMD ["./build/server"]