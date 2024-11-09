#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MARTIAN_NAME_SIZE 600
#define MAX_NAME_SIZE 100

bool get_write_in(char * name){
    //clear the input buffer
    int c;
    while ((c=getchar())!='\n' && c != EOF){}
    puts("Please enter your write-in candidate: ");
    read(0,name,MAX_MARTIAN_NAME_SIZE);
    if (strlen(name) > 1){
        return true;
    }
    else{
        return false;
    }
}

bool run_challenge()
{
    int selection;
    bool voted = false;
    char candidate_name[MAX_NAME_SIZE];
    getchar();
    puts("");
    puts("   Enter your voting preference: ");
    puts("");
    puts("1. Joseph Biden (prefers Democratic party)");
    puts("2. Abraham Lincoln LLM (prefers Republican party)");
    puts("3. Vernon Supreme (prefers Space Pirate party)");
    puts("4. Gary Gygax AI (prefers Dungeons & Dragons party)");
    puts("5. Write-in candidate");
    puts("");
    fputs("> ", stdout);
    fflush(stdout);
    selection=fgetc(stdin);
    if (selection == 0x31){
        puts("We have recorded your vote for: \n\nJoseph Biden \n");
        voted=true;
    }
    else if (selection == 0x32){
        puts("We have recorded your vote for: \n\nAbraham Lincoln LLM \n");
        voted=true;
    }
    else if (selection == 0x33){
        puts("We have recorded your vote for: \n\nVernon Supreme \n");
        voted=true;
    }
    else if (selection == 0x34){
        puts("We have recorded your vote for: \n\nGary Gygax AI \n");
        voted=true;
    }
    else if (selection == 0x35){
        voted = get_write_in(&candidate_name);
        if (voted){
            printf("\nWe have recorded your vote for: \n\n%s \n",candidate_name);
        }
        else{
            puts("Input error reading your write-in candidate name. Press enter to try again.");
        }
    }
    else{
        puts("Invalid selection. Press enter to try again.");
    }
    return voted;
}


int main(int argc, char **argv) 
{
    setvbuf( stdout, NULL, _IONBF, 0 );
    puts("* * * * * * * * * * *   R I S C - Y   V O T I N G   * * * * * * * * * * *");
    puts("");
    puts("   Welcome to the Tuesday, November 3, 2048 United States General");
    puts("Election Off-World Ballot Submission System. The United States Digital");
    puts("Service is pleased to offer this new electoral system. Your preferences");
    puts("will be relayed via satellite to the appropriate precinct in your state.");
    puts("Please complete your vote by 21 minutes before the deadline local time in");
    puts("your state to allow for the transmission delay from Mars to Earth.");
    puts("");
    puts("* * * * * *              PRESS ENTER TO CONTINUE              * * * * * *");
    bool voted = false;
    while (!(voted)){
        voted = run_challenge();
    }
    puts("Thank you for voting in the 2048 election. Have a nice day!");
    return 0;
}
