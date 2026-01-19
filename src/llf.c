#include "llf.h"
#include "windows_portability.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//more c hocus pocus, this macro can fill in the third parameter so i wouldn't have to worry about it
#define display_entry_by_name_search(headofheads,name) display_entry_by_name((headofheads),(name),0)

//saves all the ids in there
int ids[256];
int ids_counter=0;
char* headers[MAX_HEADERS];
node* headofheads;
bool loaded=false;

node* create_node(int id[], char name[], char age[],int id_length)
{
    node* new_node = malloc(sizeof(node));
    if(new_node == NULL)
    {
        return NULL;
    }

    if(id_length==1)
    {
        new_node->id=id[0];
    }
    else
    {
        int value=0;
        for(int i=id_length;i>0;i--)
        {
            value+=id[id_length-i]*pow(10,i-1);
        }
        new_node->id=value;
    }
    ids[ids_counter]=new_node->id;
    ids_counter++;

    new_node->name=malloc(strlen(name)+1);
    strcpy(new_node->name,name);

    new_node->age=malloc(strlen(age)+1);
    strcpy(new_node->age,age);

    new_node->next=NULL;

    return new_node;
}

node* append_node(node* head, int id[], char name[], char age[], int id_len)
{
    node* new_node=create_node(id,name,age,id_len);
    if(new_node==NULL)
        return new_node;
    node* end=head;
    //go to end
    while(end->next!=NULL)
    {
        end=end->next;
    }
    //assign the last node to the new node that was created
    end->next=new_node;
    return head;
}


node* insert_after_x(node* head, int key, int id[], char name[], char age[], int id_length)
{
    if(head==NULL)
        return head;
    node* temp=head;
    while(temp!=NULL)
    {
        if(temp->id==key)
        {
            break;
        }
        temp=temp->next;
    }
    if(temp==NULL)
    {
        return head;
    }
    else
    {
        node* new_node=create_node(id,name,age,id_length);
        if(new_node==NULL)
        {
            return head;
        }
        //gotta point to the next node
        new_node->next=temp->next;
        //before linking the rest of the list
        temp->next=new_node;

        return head;
    }
}

node* update_id(node* head, int old_id, int new_id)
{
    if(head==NULL)
        return head;
    node* ptr=head;
    while(ptr!=NULL)
    {
        if(ptr->id==old_id)
        {
            ptr->id=new_id;
            break;
        }
        ptr=ptr->next;
    }
    return head;
}

node* update_name_by_id(node* head, char* new_name, int name_id)
{
    if(head==NULL)
        return head;
    node* ptr=head;
    //temp array just to be able to pass it in `append_node()`
    int temp[]={99999};
    while(ptr!=NULL)
    {
        if(ptr->id==name_id)
        {
            //in case the name is longer, i'd have to malloc more so
            //its better to just create a new one and delete the old one
            
            //temporarily make a node with a different id
            append_node(head,temp,new_name,ptr->age,1);
            //delete the node with the original id
            delete_node(head,ptr->id);
            //change the id of the node to the original id
            update_id(head,temp[0],name_id);
            break;
        }
        ptr=ptr->next;
    }
    return head;
}

node* update_age_by_id(node* head, char* new_age, int age_id)
{
    //basically identical to update_name_by_id
    if(head==NULL)
        return head;
    node* ptr=head;
    int temp[]={99999};
    while(ptr!=NULL)
    {
        if(ptr->id==age_id)
        {
            append_node(head,temp,ptr->name,new_age,1);
            delete_node(head,ptr->id);
            update_id(head,temp[0],age_id);
            break;
        }
        ptr=ptr->next;
    }
    return head;
}

node* delete_node(node* head, int key)
{
    if(head==NULL)
    {
        return head;
    }
    node* temp=head;
    //prev is NULL so we can deal with
    //if the head is the key case
    node* prev=NULL;
    while(temp!=NULL)
    {
        if(temp->id==key)
        {
            break;
        }
        //prev always behind
        prev=temp;
        temp=temp->next;
    }
    if(temp==NULL)
    {
        return head;
    }
    else
    {
        //if the head is the key
        if(prev==NULL)
        {
            head=temp->next;
        }
        //every other case
        else
        {
            prev->next=temp->next;
        }
        //to delete, just replace it with the rest of the
        //array instead of making it 0
        for(int i=temp->id;i<ids_counter-1;i++)
        {
            ids[i]=ids[i+1];
        }
        ids_counter--;

        free(temp->name);
        free(temp->age);
        free(temp);

        return head;
    }
}

node* delete_first_node(node* head)
{
    if(head==NULL)
        return head;
    node* ptr=head;
    head=head->next;
    for(int i=ptr->id;i<ids_counter-1;i++)
    {
        ids[i]=ids[i+1];
    }
    ids_counter--;
    free(ptr->name);
    free(ptr->age);
    free(ptr);

    return head;
}

bool display_entry_by_id(node* head, int key)
{
    node* ptr=head;
    if(head==NULL)
    {
        printf("Error: No entries detected\n");
        return false;
    }
    while(ptr!=NULL)
    {
        if(ptr->id==key)
            break;
        ptr=ptr->next;
    }
    if(ptr==NULL)
    {
        printf("Error: Entry not found\n");
        return false;
    }
    else
    {
        printf("ID: %i",ptr->id);
        printf(" name: %s",ptr->name);
        printf(" age: %s\n",ptr->age);
        return true;
    }
}

int display_entry_by_name(node* head, char* name, int entries_ids[])
{
    bool found=false;
    bool entries_found=0;
    if(head==NULL)
    {
        printf("Error: No entries detected\n");
        return -1;
    }
    node* ptr=head;
    while(ptr!=NULL)
    {
        if(strcasecmp(ptr->name,name)==0)
        {
            if(entries_ids!=0)
                entries_ids[entries_found]=ptr->id;
            entries_found++;
            found=true;
            printf("ID: %i",ptr->id);
            printf(" name: %s",ptr->name);
            printf(" age: %s\n",ptr->age);
        }
        ptr=ptr->next;
    }
    //if head is null
    if(!found)
    {
        printf("Error: No entry found with name \"%s\"\n",name);
        return -1;
    }
    return entries_found;
}

int display_entry_by_age(node* head, char* age, int entries_ids[])
{
    bool found=false;
    bool entries_found=0;
    if(head==NULL)
    {
        printf("Error: No entries detected\n");
        return -1;
    }
    node* ptr=head;
    while(ptr!=NULL)
    {
        if(strcasecmp(ptr->age,age)==0)
        {
            if(entries_ids!=0)
                entries_ids[entries_found]=ptr->id;
            entries_found++;
            found=true;
            printf("ID: %i",ptr->id);
            printf(" name: %s",ptr->name);
            printf(" age: %s\n",ptr->age);
        }
        ptr=ptr->next;
    }
    //if head is null
    if(!found)
    {
        printf("Error: No entry found with age \"%s\"\n",age);
        return -1;
    }
    return entries_found;
}

int generate_id(void)
{
    if(ids_counter==0||ids[0]!=1)
        return 1;
    for(int i=0;i<ids_counter;i++)
    {
        bubble_sort(ids,ids_counter);
        if(ids[i+1]!=ids[i]+1)
            return ids[i]+1;
    }
    return ids[ids_counter-1]+1;
}

void display_list(node* head)
{
    node* temp=head;
    int current_id=1;
    while(temp!=NULL)
    {
        printf("%i)",current_id);
        printf(" ID: %i",temp->id);
        printf(" name: %s",temp->name);
        printf(" age: %s\n",temp->age);
        current_id++;
        temp=temp->next;
    }
}

void display_display_list(void)
{
    display_list(headofheads);
}

void present_options(void)
{
    printf("This is a simple DBMS written in c. A database management system ");
    printf("has 4 main functions:\n");
}

void search_options(void)
{

    bool done=false;
    while(!done)
    {
        printf("Search mode\n");
        printf("Search (I)d, (N)name, (A)ll, (E)xit\n");
        printf("Enter action: ");
        char search_type;
        bool accepted=false;
        while(!accepted)
        {
            scanf(" %c",&search_type);
            search_type=tolower(search_type);
            if(search_type!='i'&&search_type!='a'&&search_type!='n'&&search_type!='e')
            {
                printf("Invalid input\n");
                printf("Enter action: Search (I)d, (N)ame, (A)ll (E)xit: ");
            }
            else
            {
                accepted=true;
            }
        }
        int id;
        char name[64];
        switch(search_type)
        {
            case 'i':
                printf("Search by ID\n");
                printf("Enter ID: ");
                scanf(" %i",&id);
                display_entry_by_id(headofheads,id);
                break;
            case 'n':
                printf("Search by name\n");
                printf("Enter name: ");
                scanf(" %63[^\n]",name);
                display_entry_by_name_search(headofheads,name);
                break;
            case 'a':
                printf("Search all\n");
                display_list(headofheads);
                break;
            case 'e':
                done=true;
                break;
        }
    }
}
void insert_options(const char* csv)
{
    bool done=false;
    while(!done)
    {
        printf("Insert mode\n");
        char name_ans[64];
        printf("Enter name: ");
        //now this is some c hocus pocus
        /*
            we're using a "scanset"
            %[] is a scanset, putting in a condition in the square brackets
            63 is the max size it will read because we need space for `\0`
            ^ is a negation operator
            [^\n] means it will read everything except newline
            no need for `&` again because when passing in an array, it decays
            to a pointer to the first elemnt
        */
        scanf(" %63[^\n]",name_ans);
        char age_ans[4];
        printf("Enter age: ");
        scanf(" %3s",age_ans);
        //gotta use an array here cuz create node needs it
        //didn't need to for name and age because for some reason it works out

        //the reason is passing in an array is the same as passing in the pointer
        //to the first element 
        int generated_id[]={generate_id()};
        //the length is always 1 because i'm storing the generated id all in the
        //first element of the array
        FILE* file=fopen(csv,"a");
        if(file==NULL)
        {
            printf("Error: Couldn't open file to insert");
            continue;
        }
        fprintf(file, "%i,%s,%s\n",generated_id[0],name_ans,age_ans);
        fclose(file);
        headofheads=insert_after_x(headofheads,generated_id[0]-1,generated_id,name_ans,age_ans,1);
        printf("Data entered successfully\n");

        bool accepted=false;
        char more;
        while(!accepted)
        {
            printf("Insert more data? Y/n: ");
            scanf(" %c",&more);
            more=tolower(more);
            if(more!='y'&&more!='n')
            {
                printf("Invalid input\n");
            }
            else
            {
                accepted=true;
            }
        }
        if(more=='n')
            done=true;

        //just to keep order in the .csv i'll update it quietly after inserting
        if(!update_csv(csv,headofheads))
        {
            printf("Error: Couldn't update .csv file\n");
        }
    }
}
void update_options(const char* csv)
{
    bool done=false;
    while(!done)
    {
        bool accepted=false;
        char update_type;
        while(!accepted)
        {
            printf("Update mode\n");
            printf("Update (I)d, (N)ame, (A)ge, (E)xit \n");
            printf("Enter action: ");
            scanf(" %c",&update_type);
            update_type=tolower(update_type);
            if(update_type!='i'&&update_type!='n'&&update_type!='a'&&update_type!='e')
            {
                printf("Invalid input\n");
            }
            else
            {
                accepted=true;
            }
        }
        //false for the next while loop
        accepted=false;
        //buffer to hold in the ids lest there be duplicates
        /*
            so i have to initalize this array outside the switch statement
            because apparently it "cannot jump to case 'a' nor 'e' because
            it messes with the initialization" ok bro even though the
            buffer is used only in case 'n'
        */
        int entries_ids[ids_counter];
        int entries_num;
        bool exitted=false;
        switch(update_type)
        {
            case 'i':
                printf("Update by ID\n");
                int old_id;
                int new_id;
                //checks if id does exist
                while(!accepted)
                {
                    printf("Enter old ID (0 or less to exit): ");
                    scanf(" %i",&old_id);
                    if(old_id<=0)
                        break;
                    accepted=display_entry_by_id(headofheads,old_id);
                }
                accepted=false;
                //checks if new id conflicts with any other id
                //if they exited on the previous one, i wouldn't 
                //want to run this while loop
                if(old_id>0)
                {
                    while(!accepted)
                    {
                        printf("Enter new ID: ");
                        scanf(" %i",&new_id);
                        for(int i=0;i<ids_counter;i++)
                        {
                            if(new_id==ids[i])
                            {
                                printf("Error: ID already exists\n");
                                break;
                            }
                            accepted=true;
                        }
                    }
                    if(accepted)
                    {
                        //updates the ids array to replace the old id with new
                        for(int i=0;i<ids_counter;i++)
                        {
                            if(old_id==ids[i])
                            {
                                ids[i]=new_id;
                                break;
                            }
                        }
                        headofheads=update_id(headofheads,old_id,new_id);
                        if(!update_csv(csv,headofheads))
                        {
                            printf("Error: Couldn't update .csv file for insertion\n");
                        }
                        else
                        {
                            printf("Data updated successfully\n");
                        }
                    }
                }
                break;
            case 'n':
                printf("Update by name\n");
                char old_name[64];
                char new_name[64];
                accepted=false;
                while(!accepted)
                {
                    printf("Enter old name (e to exit): ");
                    scanf(" %63[^\n]",old_name);
                    if(strcasecmp(old_name,"e")==0)
                    {
                        exitted=true;
                        break;
                    }
                    entries_num=display_entry_by_name(headofheads,old_name,entries_ids);
                    if(entries_num>0)
                        accepted=true;
                }
                if(!exitted)
                {
                    int name_id;
                    if(entries_num>1)
                    {
                        accepted=false;
                        while(!accepted)
                        {
                            printf("Multiple entries with the same name\n");
                            printf("Please specify which one to change\n");
                            printf("Enter ID: ");
                            scanf(" %d",&name_id);
                            for(int i=0;i<entries_num;i++)
                            {
                                if(name_id==entries_ids[i])
                                {
                                    accepted=true;
                                    break;
                                }
                            }
                            if(!accepted)
                            {
                                printf("Invalid input\n");
                            }
                            
                        }
                    }
                    else
                    {
                        name_id=entries_ids[0];
                    }
                    printf("Enter new name: ");
                    scanf(" %63[^\n]",new_name);
                    update_name_by_id(headofheads,new_name,name_id);
                }
                if(!update_csv(csv,headofheads))
                {
                    printf("Error: Couldn't update .csv file\n");
                }
                else
                {
                    printf("Data updated successfully\n");
                }
                break;
            case 'a':
                printf("Update by age\n");
                char old_age[4];
                char new_age[4];
                exitted=false;
                accepted=false;
                while(!accepted)
                {
                    printf("Enter old age(e to exit): ");
                    scanf(" %3s",old_age);
                    if(strcasecmp(old_name,"e")==0)
                    {
                        exitted=true;
                        break;
                    }
                    entries_num=display_entry_by_age(headofheads,old_age,entries_ids);
                    if(entries_num>0)
                        accepted=true;
                }
                if(!exitted)
                {
                    int age_id;
                    if(entries_num>1)
                    {
                        accepted=false;
                        while(!accepted)
                        {
                            printf("Multiple entries with the same age\n");
                            printf("Please specify which one to change\n");
                            printf("Enter ID: ");
                            scanf(" %d",&age_id);
                            for(int i=0;i<entries_num;i++)
                            {
                                if(age_id==entries_ids[i])
                                {
                                    accepted=true;
                                    break;
                                }
                            }
                            if(!accepted)
                            {
                                printf("Invalid input\n");
                            }
                            
                        }
                    }
                    else
                    {
                        age_id=entries_ids[0];
                    }
                    printf("Enter new age: ");
                    scanf(" %3s",new_age);
                    update_age_by_id(headofheads,new_age,age_id);
                }
                if(!update_csv(csv,headofheads))
                {
                    printf("Error: Couldn't update .csv file\n");
                }
                else
                {
                    printf("Data updated successfully\n");
                }
                break;
            case 'e':
                done=true;
                break;
        }
    }
}
void delete_options(const char* csv)
{
    bool done=false;
    printf("Delete mode\n");
    while(!done)
    {
        char delete_type;
        bool accepted=false;
        while(!accepted)
        {
            printf("Delete by (I)D, (N)ame, (A)ge, (E)xit\n");
            printf("Enter action: ");
            scanf(" %c",&delete_type);
            delete_type=tolower(delete_type);
            if(delete_type!='i'&&delete_type!='n'&&delete_type!='a'&&delete_type!='e')
            {
                printf("Invalid Input\n");
            }
            else
            {
                accepted=true;
            }
        }
        int delete_id;
        switch(delete_type)
        {
            case 'i':
                accepted=false;
                while(!accepted)
                {
                    printf("Delete by ID\n");
                    printf("Enter ID (0 or less to exit): ");
                    //more c hocus pocus
                    /*
                        the asterisk '*' in a scanset means read but don't store, effectively ignoring the formatted scanset
                        [] scanset again
                        ^ means not
                        \\ is the literal "\" character and the first `\` is the special escape character
                        %*c just reads the last newline character and doesn't store it

                        %*[^\\n]%*c
                     */
                    scanf(" %i",&delete_id);
                    if(delete_id<=0)
                        break;
                    accepted=display_entry_by_id(headofheads,delete_id);
                }
                accepted=false;
                if(delete_id>0)
                {
                    while(!accepted)
                    {
                        char sure;
                        printf("Are you sure? Y/n: ");
                        scanf(" %c",&sure);
                        sure=tolower(sure);
                        switch(sure)
                        {
                            case 'y':
                                accepted=true;
                                break;
                            case 'n':
                                delete_id=0;
                                accepted=true;
                                break;
                            default:
                                printf("Invalid input\n");
                                break;
                        }
                    }
                    delete_node(headofheads,delete_id);
                    if(!update_csv(csv,headofheads))
                    {
                        printf("Error: Couldn't update .csv file for deletion\n");
                    }
                    else
                    {
                        printf("Data deleted successfully\n");
                    }
                }
                break;
            case 'n':
                break;
            case 'a':
                break;
            case 'e':
                done=true;
                break;
        }
    }
}

void usage(void)
{
    printf("Usage: ./tracker [.csv file]\n");
}

bool load(const char* csv)
{
    FILE* file=fopen(csv,"r");
    if(file==NULL)
    {
        printf("Error: Cannot open file\n");
        return false;
    }

    //this stores the number of bytes from the `fgets`
    const int MAX_HEADER_SIZE=32;
    char line[MAX_HEADER_SIZE];

    /*
        apparently the reason i can't use`&line` is because it decays to char(*)[64]
        while `line` decays to char* which is the correct type

        info on fgets:
        fgets(char s[], int size, FILE* file)
        return type: s on success and NULL on failure/EOF
    */
    if(!fgets(line,sizeof(line),file))
    {
        printf("Error: Couldn't load headers\n");
    }

    int separator_idx=0;
    for(int i=0;i<MAX_HEADERS;i++)
    {
        int header_len=0;
        for(int j=separator_idx;j<MAX_HEADER_SIZE;j++)
        {
            if(line[j]==','||line[j]=='\n')
            {
                separator_idx=j+1;
                break;
            }
            header_len++;
        }
        //+1 for the null terminator '\0'
        headers[i]=malloc(header_len+1);
        if(!headers[i])
        {
            printf("Error: Couldn't find free memory");
            return false;
        }
        for(int j=header_len;j>0;j--)
        {
            headers[i][header_len-j]=line[separator_idx-j-1];
        }
        headers[i][header_len]='\0';
    }

    char c;
    //buffers
    int id[4];
    char name[100];
    char age[4];
    int id_idx=0;
    int age_idx=0;
    int name_idx=0;

    int separator_counter=0;

    headofheads=NULL;

    //because of fgets, fread is now at the start of the first entry
    while(fread(&c,sizeof(c),1,file))
    {
        if(isdigit(c)&&separator_counter<=1)
        {
            //subtract 48 because 1 in ascii is 49 2 is 50 and so on
            id[id_idx]=(int)c-48;
            id_idx++;
        }
       else if(isdigit(c))
        {
            age[age_idx]=c;
            age_idx++;
        }
        else if(isalpha(c))
        {
            name[name_idx]=c;
            name_idx++;
        }
        else if(c==','||c=='\n')
        {
            separator_counter++;
        }

        if(separator_counter>=MAX_HEADERS)
        {
            age[age_idx]='\0';
            name[name_idx]='\0';
            separator_counter=0;
            if(headofheads==NULL)
                headofheads=create_node(id,name,age,id_idx);
            else
                headofheads=append_node(headofheads,id,name,age,id_idx);
            name_idx=0;
            age_idx=0;
            id_idx=0;
        }

    }
    fclose(file);

    loaded=true;
    return true;
}

//we cannot update entries in place, we will need to
//write the entire csv with the update to a new file
//then delete the old one and rename it
//i'm using the linked list structure to rewrite
//the csv file
bool update_csv(const char* csv, node* head)
{
    FILE* input=fopen(csv,"r");
    FILE* temp=fopen("temp.csv","w");
    if(temp==NULL||input==NULL)
    {
        printf("Error: Couldn't open file to update\n");
        fclose(temp);
        fclose(input);
        return false;
    }
    int MAX_LINE_SIZE=128;
    char line[MAX_LINE_SIZE];
    //copies header
    /*
        fgets write a null terminator `\0` after the LAST CHAR READ
        and NOT at the end of the buffer. 
     */
    if(!fgets(line,sizeof(line),input))
    {
        printf("Error: Couldn't read from file to update\n");
        fclose(temp);
        fclose(input);
        return false;
    }
    fclose(input);
    //this is why fputs never outputs garbage values that were in the buffer 
    //when the memory was allocated
    fputs(line,temp);

    //`fprintf`ing the file in id order
    bubble_sort(ids,ids_counter);
    //for every id in ids
    for(int i=0;i<ids_counter;i++)
    {
        //ptr to the beginning so as to not miss a node
        node* ptr=head;
        //goes through the linked list and then
        //`fprintf`s the entries in id order 
        while(ptr!=NULL)
        {
            if(ptr->id==ids[i])
            {
                fprintf(temp,"%d,%s,%s\n",ptr->id,ptr->name,ptr->age);
                break;
            }
            ptr=ptr->next;
        }
    }
    fclose(temp);
    //remove and rename are from stdio.h
    if(remove(csv)!=0)
    {
        printf("Error: Failed to remove old csv file\n");
        return false;
    }
    if(rename("temp.csv",csv)!=0)
    {
        printf("Error: Failed to rename temporary csv file\n");
        return false;
    }
    return true;
}

bool unload(void)
{
    if(!loaded)
        return false;
    while(headofheads!=NULL)
    {
        headofheads=delete_first_node(headofheads);
    }
    for(int i=0;i<MAX_HEADERS;i++)
    {
        free(headers[i]);
    }
    return true;
}

void bubble_sort(int a[], int size)
{
    for(int i=0;i<size-1;i++)
    {
        for(int j=0;j<size-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                int temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
}
