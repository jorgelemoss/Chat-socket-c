#!/bin/bash

gcc ../main/client.c -o client

mkdir -p ../build

rm -f ../build/client

mv client ../build/

docker build -t chat-socket-image -f ../Dockerfile ..
if [ $? -eq 0 ]; then
    echo "✅ Imagem 'chat-socket-image' criado com sucesso!"
else
    echo "❌ Falha ao criar a imagem."
    exit 1
fi

docker run -d -p 8096:8096 --name chat-socket-container chat-socket-image
if [ $? -eq 0 ]; then
    echo "🚀 Container 'chat-socket-container' iniciado em segundo plano na porta 8096!"
    echo "ℹ️ Use 'docker logs chat-socket-container' para ver os logs."
else
    echo "❌ Falha ao iniciar o container."
    exit 1
fi

echo ""
echo "✅ O client foi gerado e movido para a pasta 'build/'."
echo "👉 Para executar o client, use o comando abaixo:"
echo ""
echo "👉 chmod +x ./build/client (Caso seja necessário)"
echo ""
echo "  cd build && ./client"
echo ""