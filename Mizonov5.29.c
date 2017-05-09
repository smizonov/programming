# include <stdio.h>
# include <stdlib.h>
 int main()
   {
   int *n, *p, len, i, x, j, q, *a;
   q=-1;
   printf(" Введите количество элементов\n");
   scanf("%d",&len);
   p=(int*)malloc((len)*sizeof(int));
   printf("Введите Х\n");
   scanf("%d", &x);
     for(i=0;i<len; i++)
     {
    printf("Введите %d-ый элемент ", i+1);
    scanf("%d",&a[i]);
     }
    if (x==a[len/2])
    q=len/2+1;
    else
      {
      if(x<a[len/2])
       {
       for (j=0;j<(len/2);j++)
         {
         if(a[j]==x)
        q=j+1; 
          }
        }
        else
           {
            for(j=len/2;j<=len;j++)
            {
             if(a[j]==x)
             q=j+1;  
             }    
            }
        }
    printf("Номер индекса= %d\n",q);
    }         
             