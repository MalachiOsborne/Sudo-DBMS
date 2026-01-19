#include "dbmsf.h"
#include "llf.h"
#include "windows_portability.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
