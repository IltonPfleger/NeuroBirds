# Neuro Birds

![C](https://badgen.net/badge/MADE-WITH/C)
![LICENSE](https://badgen.net/badge/LICENSE/MIT/green)
![SDL](https://badgen.net/badge/SDL/SDL2/red)

Uma aplicação de algoritmos genéticos que se utilizam de redes neurais para aprender a jogar o jogo Flappy Bird.

<div align="center">
    <img width="50%" src="https://github.com/IltonPfleger/Neuro-Bird/blob/main/preview.gif">
</div>

# Sobre o Projeto

Objetivo: Criar um sistema que simule a evolução natural selecionando, mutando e cruzando os melhores indivíduos a cada geração de pássaros. 

Todo o projeto é feito apenas com **SDL/SDL2** e **C** puro, utilizando algumas estruturas dinâmicas como listas encadeadas para os canos e um array para os os pássaros.

## Sobre a Rede
A rede neural aplicada a cada pássaro possui apenas uma entrada, 3 neurônios ocultos e uma saída.

* A entrada da rede recebe apenas um valor que aponta a distância do pássaro ao centro da próxima passagem.
  
* O objetivo da rede é minimizar a entrada uma vez que, se ela for zero, o passaro está entre os canos.
  
* Para a saída, se ela for maior do que zero, o pássaro pula, caso contrário, não pula.

* A função de ativação usada durante todo o processo é a ReLU.

## Sobre o algoritmo genético
A seleção dos pássaros que passam para as próximas gerações é feita através de uma função de fitness que faz uma escolha aleatória levando em consideração o quão longe o pássaro foi, e usando isso como peso para probabilidade de que ele seja selecionado.

* Após a seleção, os pássaros passam por algumas modificações para adicionar certa aleatoriamente ao sistema.

* Existe uma probabilidade de 85% de somar um número aleatório em cada peso da rede daquele pássaro.
  
* Existe uma probabilidade de 50% de dois pássaros trocarem pesos por um processo de **Crossing Over**.
  
* Essas probabilidades podem ser alteradas no arquivo **birds.h** em suas respectivas funções.

## Outras informações:

* Pela simplicidade do jogo, é bastante comum que, dependendo do número de pássaros da geração, que diversos indivíduos nasçam sabendo jogar o jogo.

* Os parâmetros gerais do jogo podem ser alterados nas definições do arquivo **main.c**

* Fique a vontade para mandar ideias, sugestões, dúvidas e requests. 
