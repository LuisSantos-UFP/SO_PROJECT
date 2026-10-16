# SO_PROJECT

# Minishell em C — Sistemas Operativos

## 1. Descrição do Projeto

Este projeto consiste no desenvolvimento de uma *Minishell* em C, para ambientes Linux/POSIX, no âmbito da unidade curricular de Sistemas Operativos.

O objetivo é desenvolver uma shell simples capaz de interpretar e executar comandos, gerir processos, realizar redirecionamentos de entrada e saída, permitir a comunicação entre processos através de pipes e implementar funcionalidades de concorrência com threads e semáforos.

O projeto está dividido em duas fases, abrangendo conceitos fundamentais de sistemas operativos e programação concorrente.

## 2. Objetivos

Os principais objetivos deste projeto são:

- Compreender a criação e gestão de processos.
- Implementar comandos internos e executar programas externos.
- Permitir a execução de comandos em primeiro e segundo plano.
- Implementar redirecionamentos de entrada e saída.
- Estabelecer comunicação entre processos através de pipes.
- Compreender e utilizar sinais do sistema operativo.
- Desenvolver aplicações multithreaded.
- Implementar mecanismos de sincronização com semáforos POSIX.
- Aplicar boas práticas de programação e gestão de memória.

## 3. Tecnologias Utilizadas

- **Linguagem:** C (C11).
- **Sistema operativo:** Linux/POSIX.
- **Compilador:** GCC.
- **Processos:** `fork()`, `execvp()` e `waitpid()`.
- **Redirecionamentos:** `open()`, `close()` e `dup2()`.
- **Comunicação entre processos:** Pipes POSIX.
- **Threads:** Biblioteca POSIX Pthreads.
- **Sincronização:** Semáforos POSIX.
- **Gestão de memória:** `malloc()`, `calloc()` e `free()`.
- **Validação de memória:** Valgrind.

## 4. Funcionalidades

### Fase 1 — Funcionalidades Básicas da Shell

A primeira fase centra-se na implementação do ciclo principal da shell e na execução de comandos.

#### 4.1. Ciclo Principal (REPL)

- Apresentação de um prompt de comandos.
- Leitura dos comandos introduzidos pelo utilizador.
- Utilização do módulo de parsing fornecido.
- Gestão e libertação da memória utilizada.

#### 4.2. Execução de Comandos

A shell deverá permitir a execução de comandos externos, como `ls`, `date` e `ps`, bem como disponibilizar os seguintes comandos internos:

| Comando | Descrição |
|---------|-----------|
| `cd` | Altera a diretoria de trabalho atual. |
| `pwd` | Apresenta o caminho absoluto da diretoria atual. |
| `exit` | Termina a execução da shell. |
| `history` | Apresenta o histórico de comandos. |
| `echo` | Apresenta texto no terminal. |
| `help` | Apresenta informações sobre os comandos disponíveis. |

#### 4.3. Execução em Segundo Plano

Permite executar comandos em *background* através do operador `&`, devolvendo imediatamente o controlo ao utilizador.

#### 4.4. Redirecionamentos

A shell deverá suportar os seguintes operadores:

- `>` — Redireciona a saída para um ficheiro, substituindo o conteúdo existente.
- `>>` — Adiciona a saída ao final de um ficheiro.
- `<` — Utiliza um ficheiro como entrada.
- `2>` — Redireciona a saída de erro para um ficheiro.

#### 4.5. Pipes e Gestão de Processos

Permite ligar vários comandos através do operador `|`, encaminhando a saída de um processo para a entrada do seguinte.

Inclui também o tratamento de sinais e a gestão de processos terminados, de forma a evitar a acumulação de processos zombie.

### Fase 2 — Concorrência e Sincronização

A segunda fase introduz funcionalidades avançadas, recorrendo a threads e mecanismos de sincronização.

#### 4.6. Comando `indexer`

Implementação de um comando interno para analisar recursivamente uma diretoria.

Deverá permitir:

- Contabilizar o número total de ficheiros.
- Contabilizar o número total de pastas.
- Calcular o tamanho total dos ficheiros em bytes.
- Agrupar ficheiros por extensão.
- Distribuir o trabalho por várias threads.

#### 4.7. Sincronização do `indexer`

Utilização de semáforos POSIX para proteger os dados partilhados entre threads, garantindo a consistência dos resultados e evitando condições de corrida (*race conditions*).

#### 4.8. Comando `pbackup`

Implementação de um sistema de cópias de segurança paralelas através de um modelo produtor-consumidor.

O comando deverá:

- Percorrer recursivamente a diretoria de origem.
- Armazenar os caminhos dos ficheiros num buffer circular limitado.
- Utilizar uma thread produtora para identificar os ficheiros.
- Utilizar várias threads consumidoras para copiar os ficheiros.
- Sincronizar o acesso ao buffer através de semáforos e mecanismos de exclusão mútua.
- Garantir a finalização correta de todas as threads.

## 5. Compilação

O projeto deverá ser compilado com o GCC, utilizando as opções indicadas no enunciado:

```bash
gcc -Wall -Wextra -pedantic -pthread -std=c11 *.c -o minishell
