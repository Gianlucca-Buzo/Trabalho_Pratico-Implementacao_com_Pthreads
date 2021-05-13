# Trabalho_Pratico-Implementacao_com_Pthreads
### Membros do grupo: Darlei Matheus Schmegel, Dauan Ghisleni Zolinger, Gianlucca de Mendonça Buzô, Heitor Felipe Matozo dos Santos e Letícia Pegoraro Garcez.

# Comandos para rodar desde o zero
`gcc -c -pthread pokeLib.c -o libpokeLib.o`
`ar rv libpokeLib.a libpokeLib.o`
`gcc  Fibonnaci.c -L. -l libpokeLib.a -o Fibonnaci.bin -lm`