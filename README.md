# Trabalho_Pratico-Implementacao_com_Pthreads
### Membros do grupo: Darlei Matheus Schmegel, Dauan Ghisleni Zolinger, Gianlucca de Mendonça Buzô, Heitor Felipe Matozo dos Santos e Letícia Pegoraro Garcez.

# Comandos para compilar separados
`$ gcc -g c pokeLib.c`

`$ gcc -g Fibonnaci.c pokeLib.o -lpthread -o Fibonnaci`

# Comando para compilar tudo junto
`$ gcc -g Fibonnaci.c pokeLib.c -lpthread -o Fibonnaci`

# Comandos para rodar desde o zero
`gcc -c -pthread pokeLib.c -o libpokeLib.o`
`ar rv libpokeLib.a libpokeLib.o`
`gcc  Fibonnaci.c -L. -l libpokeLib.a -o Fibonnaci.bin -lm`