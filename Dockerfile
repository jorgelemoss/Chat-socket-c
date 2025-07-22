FROM ubuntu:latest

WORKDIR /app

RUN apt update && apt install -y gcc

COPY . .

RUN chmod +x makeexec.sh

RUN ./makeexec.sh

EXPOSE 8096

CMD ["./build/server"]