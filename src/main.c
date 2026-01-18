#include "llf.h"
#include "windows_portability.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if(argc<2 || argc>2)
    {
        usage();
        return -1;
    }
    bool loaded=load(argv[1]);
    if(!loaded)
    {
        return -1;
    }
    else
    {
        present_options();
    }

    bool done=false;
    while(!done)
    {
        printf("(S)earch, (I)nsert, (U)pdate, (D)elete and (E)xit\n");
        printf("Enter action: ");
        char ans;
        bool accepted=false;
        while(!accepted)
        {
            /*
                the space before the `%c` is actually very crucial because it "eats
                any newline characters that the user enters when they enter a wrong
                letter.
                when u type in "r" its actually "r\n" which scanf reads but it only
                putts the "r" in the ans because that's how large the buffer is and
                leaves the "\n" to the next scanf
            */
            scanf(" %c",&ans);
            ans=tolower(ans);
            //use && because if u want to use || it would be == instead of !=
            if(ans!='s'&&ans!='i'&&ans!='u'&&ans!='d'&&ans!='e')
            {
                printf("Invalid input\n");
                printf("Enter action: (S)earch, (I)nsert, (U)pdate, (D)elete, (E)xit: ");
            }
            else
            {
                accepted=true;
            }
        }

        switch (ans)
        {
            case 's':
                search_options();
                break;
            case 'i':
                insert_options(argv[1]);
                break;
            case 'u':
                update_options(argv[1]);
                break;
            case 'd':
                delete_options(argv[1]);
                break;
            case 'e':
                done=true;
                break;
        }
    }

    unload();


    return 0;
}
