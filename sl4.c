#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#define N 3
#define SIZE 8

struct node{
    int data;
    bool mark;
    int refCount;
    int index;
    struct node *link[N];
};

struct heap{
    struct node memory;
    struct heap *next;
};

struct heap *head=NULL;
struct node *ptr[SIZE];
int memory_freed=0;
int nodes_deleted=0;
int node_index=0;

struct node* insertBeg(int size,struct node *link1,struct node *link2,struct node *link3)
{
    struct heap *ptr=malloc(sizeof(struct heap));
    ptr->memory.data=size;
    ptr->memory.mark=false;
    ptr->memory.link[0]=link1;
    ptr->memory.link[1]=link2;
    ptr->memory.link[2]=link3;
    ptr->memory.refCount=0;
    ptr->memory.index= (++node_index);
    ptr->next=head;
    head=ptr;
    
    struct node *temp=&(ptr->memory);
    
    if(link1 != NULL)
    {
        link1->refCount +=1;
    }
    if(link2 != NULL)
    {
        link2->refCount +=1;
    }
    if(link3 != NULL)
    {
        link3->refCount +=1;
    }
    
    return temp;
}

void printList() // This function will display the nodes present in the heap
{
    struct heap *temp=head;

    while(temp!=NULL)
    {
        printf("(%d , refc=%d) ->",temp->memory.data,temp->memory.refCount);
        temp=temp->next;
    }
    printf("NULL\n");
}

void allocate(struct node *root)
{
    (root)->refCount +=1;
}

void printReference(struct node *root)
{
    if(root!=NULL)
    {
        printf("%d(rfc=%d) ",root->data,root->refCount);
        printReference(root->link[0]);
        printReference(root->link[1]);
        printReference(root->link[2]);
    }
}

void decIndex(struct heap *ptr)
{
    struct heap *temp=head;
    
    while(temp != ptr)
    {
        temp->memory.index -=1;
        temp=temp->next;
    }
}

void garbageCollect(struct node *ptr[],int n)
{
    int j=n-1;
    struct heap *temp=head;
    struct heap *prev=NULL;
    
    while(temp!=NULL)
    {
        if(temp->memory.refCount==0)
        {
            for(int i=0;i<N;i++)
            {
                if(temp->memory.link[i] != NULL)
                {
                    temp->memory.link[i]->refCount -=1; 
                }
            }
            decIndex(temp); 
            prev->next=temp->next;
            memory_freed += temp->memory.data;
            nodes_deleted +=1;
            free(temp);
            temp=prev;
            ptr[j]=NULL;
        }
        prev=temp;
        temp=temp->next;
        j--;
    }
}

void markNodes(struct node *root,int i,int j)
{
    struct node *current, *pre;

    current = root;
        
    while (current != NULL) 
    {
  
        if (current->link[i] == NULL) 
        {
            current->mark=true;
            current = current->link[j];
        }   
        else 
        {
            pre = current->link[i];
            while ((pre->link[j] != NULL) && (pre->link[j] != current))
                pre = pre->link[j];
  
            if (pre->link[j] == NULL) 
            {
                pre->link[j] = current;
                current = current->link[i];
            }
            else 
            {
                pre->link[j] = NULL;
                current->mark=true;
                current = current->link[j];
            } 
        }
    }   
}

void sweepnodes()
{
    int j=SIZE-1;
    struct heap *temp=head;
    struct heap *prev=NULL;
    
    while(temp!=NULL)
    {
        if(temp->memory.mark==false)
        {
            for(int i=0;i<N;i++)
            {
                if(temp->memory.link[i] != NULL)
                {
                    temp->memory.link[i]->refCount -=1;
                }
            }
            prev->next=temp->next;
            memory_freed += temp->memory.data;
            nodes_deleted +=1;
            free(temp);
            temp=prev;
            ptr[j]=NULL;
        }
        prev=temp;
        temp=temp->next;
        j--;
    }
}

void AdjMatrix(int n)
{
    struct heap *temp=head;
    int adj[n+1][n+1];
    
    for(int i=0;i<n+1;i++)
    {
        for(int j=0;j<n+1;j++)
            adj[i][j]=0;
    }
    
    for(int i=0;i<n+1 && temp!=NULL ;i++)
    {
        adj[n-i][0]=temp->memory.data;
        adj[0][n-i]=temp->memory.data;
        temp=temp->next;
    }
    temp=head;
    
    while(temp!=NULL)
    {
        for(int k=0;k<N;k++)
        {
            if(temp->memory.link[k] != NULL)
            {
                int i=temp->memory.index;
                int j=temp->memory.link[k]->index;
                adj[i][j]=1;
            }
        }
        
        temp=temp->next;
    }
    
    for(int i=0;i<n+1;i++)
    {
        for(int j=0;j<n+1;j++)
            printf("%d ",adj[i][j]);
        printf("\n");
    }
}

int main()
{
    int choice;
    
    // creating nodes 
    ptr[0]=insertBeg(10,NULL,NULL,NULL);
    ptr[1]=insertBeg(9,NULL,NULL,NULL);
    ptr[2]=insertBeg(2,NULL,NULL,NULL);
    ptr[3]=insertBeg(8,ptr[1],NULL,NULL);
    ptr[4]=insertBeg(1,ptr[2],ptr[1],ptr[0]);
    ptr[5]=insertBeg(3,ptr[3],ptr[0],NULL);
    ptr[6]=insertBeg(7,ptr[4],ptr[3],NULL);
    ptr[7]=insertBeg(5,ptr[4],NULL,NULL);
    
    struct node *root1,*root2; // linking root1 and root2 with the given nodes
    root1=ptr[7];
    allocate(root1);
    root2=ptr[4];
    allocate(root2);
    
    do
    {
        printf("\n");
        printf("0 - Exit.\n");
        printf("1 - Reference Counting Method.\n");
        printf("2 - Mark and Sweep Method.\n");
        printf("Enter choice : ");
        scanf("%d",&choice);
        
        switch(choice)
        {
            case 0:
            break;
            
            case 1:
            
                printf("\nThe bytes occupied by nodes in the heap are : \n");
                printList();
                printf("------------------------------------------\n");
                
                printf("The nodes in the heap are : \n");
                for(int i=SIZE-1;i>=0;i--)
                {
                    printReference(ptr[i]);
                    printf("\n");
                }
                
                printf("------------------------------------------\n");
                printf("The adjacency matrix before GC is : \n");
                AdjMatrix(SIZE);
                
                
                printf("------------------------------------------\n");
                printf("The root nodes are : \n");
                printf("root1->");
                printReference(root1);
                printf("\nroot2->");
                printReference(root2);
                printf("\n------------------------------------------\n");
    
                printf("After garbage collection, the nodes are : \n");
    
                garbageCollect(ptr,SIZE);
    
                for(int i=SIZE-1;i>=0;i--)
                {
                    printReference(ptr[i]);
                    if(ptr[i]!=NULL)
                        printf("\n");
                }
                printf("------------------------------------------\n");
                printf("The adjacency matrix after GC is : \n");
                AdjMatrix(SIZE-nodes_deleted);
                
                printf("------------------------------------------\n");
                printf("After deleting(free) the garbage nodes,\n");
                printf("Total memory freed = %d bytes.\n",memory_freed);
                printf("Total nodes deleted = %d.\n",nodes_deleted);
                printf("\nThe bytes occupied by nodes after GC in the heap are : \n");
                printList();
                
            break;
            
            case 2:
                
                printf("\nThe bytes occupied by nodes in the heap are : \n");
                printList();
                printf("------------------------------------------\n");
                markNodes(root1,0,1);
                markNodes(root2,0,2);
    
                sweepnodes();
                
                printf("\nAfter mark sweep method : \n");
                for(int i=SIZE-1;i>=0;i--)
                {
                    printReference(ptr[i]);
                    if(ptr[i]!=NULL)
                        printf("\n");
                }
                printf("\nThe adjacency matrix after GC is : \n");
                AdjMatrix(SIZE-nodes_deleted);
                printf("------------------------------------------\n");
                printf("\nTotal memory freed = %d bytes.\n",memory_freed);
                printf("Total nodes deleted = %d.\n",nodes_deleted);
                printf("\nThe bytes occupied by nodes after mark-sweep in the heap are : \n");
                printList();
            
            break;
            
            default:
                printf("Invalid choice.");
        }
    }while(choice!=0);

    return 0;
}