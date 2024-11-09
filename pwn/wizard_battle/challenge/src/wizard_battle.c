#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 60

void cast_spell(){
    //clear the input buffer
    char spellcode[BUFFER_SIZE];
    int c;
    while ((c=getchar())!='\n' && c != EOF){}
    puts("Please enter the spellcode you wish to run: ");
    read(0,spellcode,BUFFER_SIZE);
    puts("Thegrer casts counterspell and corrupts your spellcode!");
    spellcode[17]='\00';
    spellcode[18]='\00';
    spellcode[30]='\00';
    spellcode[31]='\00';
    void (*spell)() = (void(*)())spellcode;
    spell();
    return;
}


int main(int argc, char **argv) 
{
    setvbuf( stdout, NULL, _IONBF, 0 );
    alarm(60);
    puts("* * * * * * * * * * *   W I Z A R D   B A T T L E   * * * * * * * * * * *");
    puts("");
    puts("   Your party is on a mission to stop the evil wizard Thegrer from");
    puts("automatically opting your hamlet into scrying and clairvoyance spells for");
    puts("the purpose of personalized product hawking at the weekly market.");
    puts("One by one as you scale the adobe steps to Thegrer\'s mountaintop chateau,");
    puts("Thegrer\'s acrobats have felled your party members in a hail of dark");
    puts("patterns and legal jargon. Now, only your arcane knowledge can stop him.");
    puts("Deftly navigating a hall of smoke and mirrors, you find Thegrer drafting");
    puts("his latest privacy policy. Your mission: get shell and opt out! You have");
    puts("time for only one spell before the plot gets weirder.");
    puts("");
    puts("* * * * * *              PRESS ENTER TO CONTINUE              * * * * * *");
    cast_spell();
    return 0;
}
