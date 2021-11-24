//Assignment 2 - Convex Hull
//Archit Agarwal(18CS10006)
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct point
{
	double x,y;
}point;

typedef struct line_segment
{
	point start;
	point end;
}segment;

typedef struct arc
{
	point center;
	double begin,finish;
}arc;
 
//Mergesort used for sorting points acc. to x-coordinates
/****************Mergesort Start********************/
void merge(point* arr, int l, int m, int r)  
{  
    int i, j, k;  
    int n1 = m - l + 1;  
    int n2 = r - m;  

    point L[n1], R[n2];  
  
    for (i = 0; i < n1; i++)  
        L[i] = arr[l + i];  
    for (j = 0; j < n2; j++)  
        R[j] = arr[m + 1 + j];  

    i = 0;   
    j = 0;   
    k = l; 
    while (i < n1 && j < n2) 
    {  
        if (L[i].x <= R[j].x) 
        {  
            arr[k] = L[i];  
            i++;  
        }  
        else 
        {  
            arr[k] = R[j];  
            j++;  
        }  
        k++;  
    }  
    while (i < n1) 
    {  
        arr[k] = L[i];  
        i++;  
        k++;  
    }  
    while (j < n2) 
    {  
        arr[k] = R[j];  
        j++;  
        k++;  
    }  
}  

void mergesort(point* arr, int l, int r)  
{  
    if (l < r) 
    {  
        int mid = l + (r - l) / 2;  

        mergesort(arr, l, mid);  
        mergesort(arr, mid + 1, r);  
  
        merge(arr, l, mid, r);  
    } 
}  
/****************Mergesort End********************/  

/****************Stack Creation********************/
typedef struct stack 
{ 
    point data; 
    struct stack* next; 
}stack; 
  
stack* newNode(point data) 
{ 
    stack* stackNode = (stack*)malloc(sizeof(stack)); 
    stackNode->data = data; 
    stackNode->next = NULL; 
    return stackNode; 
} 
  
int isEmpty(stack* root) 
{ 
    return !root; 
} 
  
void push(stack** root, point data) 
{ 
    stack* stackNode = newNode(data); 
    stackNode->next = *root; 
    *root = stackNode; 
} 
  
void pop(stack** root) 
{  
    stack* temp = *root; 
    *root = (*root)->next;  
    free(temp);  
} 
  
point top(stack* root) 
{  
    return root->data; 
} 

point second(stack* root)
{
    return root->next->data;
}
/****************Stack Creation********************/

//checking for turn by side determinant formula
int right_turn(point p1, point p2, point p3)
{
    double side = (p3.x*p2.y)-(p2.x*p3.y)+(p2.x*p1.y)-(p1.x*p2.y)+(p1.x*p3.y)-(p3.x*p1.y); 
    if(side<=0)
        return 1;
    return 0;
}

//Convex Hull Finding by Graham Scan
int CH(point* S, int n, int flag, point* H)
{
    stack* s = NULL;
    int i=2,size=2;
    push(&s,S[0]);
    if(n!=1)
        push(&s,S[1]);
    if(n==1)
        size--;
    if(flag)
    {
        while(i<n)
        {
            point tp = top(s);
            point sec = second(s);
            if(right_turn(S[i],tp,sec))
            {
                push(&s,S[i]);
                size++;
            }
            else
            {
                while(1)
                {
                    if(!right_turn(S[i],tp,sec))
                    {
                        pop(&s);
                        size--;
                    }
                    else
                        break;
                    if(s->next==NULL)
                        break;
                    tp = top(s);
                    sec = second(s);
                }
                push(&s,S[i]);
                size++;
            }
            i++;
        }
        for(int j=size-1;j>=0;j--)
        {
            H[j] = top(s);
            pop(&s);
        }
    }
    else
    {
        while(i<n)
        {
            point tp = top(s);
            point sec = second(s);
            if(!right_turn(S[i],tp,sec))
            {
                push(&s,S[i]);
                size++;
            }
            else
            {
                while(1)
                {
                    if(right_turn(S[i],tp,sec))
                    {
                        pop(&s);
                        size--;
                    }
                    else
                        break;
                    if(s->next==NULL)
                        break;
                    tp = top(s);
                    sec = second(s);
                }
                push(&s,S[i]);
                size++;
            }
            i++;
        }
        for(int j=0;j<size;j++)
        {
            H[j] = top(s);
            pop(&s);
        }
    }
    return size-1;
}

void printPoints(point* arr, int size)  
{    
    for(int i=0;i<size;i++)  
        printf("    %0.15lf %0.15lf\n",arr[i].x,arr[i].y);  
} 

void contzone(point* UH, int u, point* LH, int l, double r, segment* T, arc* A)
{
    double pi = 3.141592653589793;
    double theta,prev_theta;

    A[0].begin = pi;
    for(int i=0;i<=u;i++)
    {
        A[i].center = UH[i];
        theta = atan2(UH[i+1].y-UH[i].y,UH[i+1].x-UH[i].x);
        theta = theta + pi/2;
        if(theta<0)
            theta += 2*pi;
        if(i!=u)
        {
            T[i].start.x = UH[i].x + r*cos(theta);
            T[i].start.y = UH[i].y + r*sin(theta);
            A[i].finish = theta;  
        }
        if(i!=0)
        {
            T[i-1].end.x = UH[i].x + r*cos(prev_theta);
            T[i-1].end.y = UH[i].y + r*sin(prev_theta);
            A[i].begin = prev_theta;
        }
        prev_theta = theta;
    }
    A[u].finish = 0;

    A[u+1].begin = 0;
    for(int i=0;i<=l;i++)
    {
        A[i+u+1].center = LH[i];
        theta = atan2(LH[i+1].y-LH[i].y,LH[i+1].x-LH[i].x);
        theta = theta + pi/2;
        if(theta>0)
            theta -= 2*pi;
        if(i!=l)
        {
            T[i+u].start.x = LH[i].x + r*cos(theta);
            T[i+u].start.y = LH[i].y + r*sin(theta);
            A[i+u+1].finish = theta; 
        }
        if(i!=0)
        {
            T[i+u-1].end.x = LH[i].x + r*cos(prev_theta);
            T[i+u-1].end.y = LH[i].y + r*sin(prev_theta);
            A[i+u+1].begin = prev_theta;
        }
        prev_theta = theta;
    }
    A[u+l+1].finish = -1*pi;
}

void printcontzone(int u, int l, segment* T, arc* A)
{
    printf("\n+++ The containment zone\n");
    printf("--- Upper section\n");
    for(int i=0;i<=u;i++)
    {
        printf("    Arc     : (%0.15lf,%0.15lf) From %0.15lf to %0.15lf\n",A[i].center.x,A[i].center.y,A[i].begin,A[i].finish);
        if(i!=u)
            printf("    Tangent : From (%0.15lf,%0.15lf) to (%0.15lf,%0.15lf)\n",T[i].start.x,T[i].start.y,T[i].end.x,T[i].end.y);
    }
    printf("--- Lower section\n");
    for(int i=u+1;i<=u+l+1;i++)
    {
        printf("    Arc     : (%0.15lf,%0.15lf) From %0.15lf to %0.15lf\n",A[i].center.x,A[i].center.y,A[i].begin,A[i].finish);
        if(i!=u+l+1)
            printf("    Tangent : From (%0.15lf,%0.15lf) to (%0.15lf,%0.15lf)\n",T[i-1].start.x,T[i-1].start.y,T[i-1].end.x,T[i-1].end.y);
    }
}

int main()
{
	int n;
	double r;
	scanf("%d",&n);
	scanf("%lf",&r);

    point* circle;
    circle = (point *)malloc(n*sizeof(point));
	for(int i=0;i<n;i++)
		scanf("%lf %lf",&circle[i].x,&circle[i].y);

	mergesort(circle,0,n-1);
    printf("ss\n");
    printf("\n+++ Circles after sorting\n");
	printPoints(circle,n);

    point* upperhull;
    point* lowerhull;
    upperhull = (point *)malloc(n*sizeof(point));
    lowerhull = (point *)malloc(n*sizeof(point));
    int upperedge, loweredge;
    upperedge = CH(circle,n,1,upperhull);
    loweredge = CH(circle,n,0,lowerhull);
    printf("\n+++ Upper hull\n");
    printPoints(upperhull,upperedge+1);
    printf("\n+++ Lower hull\n");
    printPoints(lowerhull,loweredge+1);

    int hullsize = upperedge + loweredge;
    segment* contsegments;
    arc* contarcs;
    contsegments = (segment *)malloc(hullsize*sizeof(segment));
    contarcs = (arc *)malloc((hullsize+2)*sizeof(arc));
    contzone(upperhull,upperedge,lowerhull,loweredge,r,contsegments,contarcs);
    printcontzone(upperedge,loweredge,contsegments,contarcs);

	return 0;
}