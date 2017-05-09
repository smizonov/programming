#include <iostream>

using namespace std;

int main()
{
    int n, x, *arr, left, right,flag=1, midd=0;
    cout<<"Введите размер массива: ";
    cin>>n;
    cout<<endl;
    arr=new int[n];
    for (int i=0;i<n; i++)
    {
      cout<<"Введите "<<i+1<<"-ый элемент: ";
      cin>>arr[i];
    }
   cout<<"Введите число x: ";
   cin>>x;

   left=0;
   right=n;

  while(flag==1)
  {
       midd = (left + right) / 2;

       if (x < arr[midd])
           right = midd - 1;
       else if (x > arr[midd])
           left = midd + 1;
       else
            {
           flag=0;
           cout<<"Ваше число находится в ячейке с номером: "<<midd+1<<endl;
             }
           if (left > right)
           {
             flag=0;
               cout<<"Нет такого числа"<<endl;
           }

 }

}

