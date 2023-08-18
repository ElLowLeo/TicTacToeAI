#!/bin/bash

while true; do
    echo "Qual versão do jogo deseja abrir?"
    echo "1. Jogo da velha em Rust"
    echo "2. Jogo da velha em C"
    echo "3. Sair"
    read choice

    case $choice in
        1)
            while true; do
                read -p "Pressione Enter para continuar. . ."
                ./tictactoeRUST
            done
            ;;
        2)
            while true; do
                read -p "Pressione Enter para continuar. . ."
                ./tictactoeC
            done
            ;;
        3)
            echo "Saindo"
            break
            ;;
        *)
            echo "Escolha inválida, tente novamente"
            ;;
    esac
done
