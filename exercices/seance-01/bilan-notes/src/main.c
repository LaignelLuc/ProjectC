#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int notes[5];
    int somme = 0;
    int max;
    int notes_valides = 0;
    int i;
    for (int i; i < 5; i++){
    	scanf("%d",&notes[i]);
    }
    for (int i; i < 5; i++){
    	if(notes[i] > max){
        	max = notes[i];
        }
        if(notes[i] >= 10){
        	notes_valides++;
        }
        somme += notes[i];
    }
   
    printf("somme = %d\n", somme);
    printf("moyenne = %d\n", somme / 5);
    printf("max = %d\n", max);
    printf("notes valides = %d\n", notes_valides);

    return EXIT_SUCCESS;
}
