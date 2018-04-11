//  Chang Xu
//  lab2.c
//  lecture section 01
//  lab section 02
//
//

#include <stdio.h>
#include <strings.h>
#include <math.h>
#include <stdbool.h>

#define DEBUG true

int main()
{
  printf("CS 350 Lab 2 for %s\n%s\n%s\n\n", "Chang Xu","lecture section 01", "lab section 02");
  int baseTenValue, base;
  printf("Enter an integer and base:\n");
  printf("(int >= 1 and 2 <= base <= 36 or we quit): >");
  scanf("%d %d", &baseTenValue, &base);
  //test
  printf("%d,%d",baseTenValue,base);

  int n, k, q;

  while (baseTenValue>=1 && base>=2 && base<=36)
  {

      n=0;
      k=1;

      //n is the power of the base, k is k*base^(n-1)

      //decide n
      while ((int)(pow(base,n))<=baseTenValue)
      {
          n++;
          if(DEBUG){printf("inching up n, n=%d, because %d<=%d\n",n,((int)(pow(base,n))),baseTenValue );}
      }
      if(DEBUG){printf("done inching up n\n\n");}

      q=n;

      int digit[n];
      //set it all to zero
      for (int i=0; i<n; i++) {
          digit[i]=0;
      }

      //decide k
      while ( (k * ((int)(pow(base,(n-1))))) <= baseTenValue)
      {
          if(DEBUG){printf("%d * %d <= %d \n",k, ((int)(pow(base,(n-1)))), baseTenValue );}
          k++;
      }

      digit[n-1]=k-1;

      if(DEBUG){printf("%d = %d - %d\n",(baseTenValue - ((k-1)*((int)(pow(base,(n-1)))))), (baseTenValue),(((k-1)*((int)(pow(base,(n-1)))))));}


      baseTenValue = baseTenValue - ((k-1)*((int)(pow(base,(n-1)))));


      n=n-1;
      k=1;



      while (n-1 >= 0)
      {
        //same thing copyies

        //decide k
        while ( (k * ((int)(pow(base,(n-1))))) <= baseTenValue)
        {
            if(DEBUG){printf("decide k %d * %d <= %d\n",k,(((int)(pow(base,(n-1))))),baseTenValue);}
            k++;
        }
        //assign the value
        if (baseTenValue<=0)
        {
          if(DEBUG){printf("assigning val %d at index %d because %d <= 0\n",0,(n-1),baseTenValue);}
            digit[(n-1)]=0;
        }
        else
        {
          if(DEBUG){printf("assigning val %d at index %d because %d > %d\n",(k-1),(n-1),(baseTenValue),(0));}
            digit[(n-1)]=(k-1);
            if(DEBUG){printf("arr[%d]=%d\n",(n-1),(digit[k-1]));}
        }
        //subtraction


        //if(DEBUG){printf("ass");}
        baseTenValue=baseTenValue-(k-1)*(int)(pow(base,(n-1)));


        n=n-1;
        k=1;
        //same thing emdes
      }

      //test
     // printf("%d,%d",n,k);

      //test ends

      printf("pls let %d not be lies\n\n",q);

      for (int i=0; i<q; i++) {
          printf("%d\n",digit[i]);
      }



      char prt_digits[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


      //loop through array backwards at index i
        //print prt_digits[array[i]]



      printf("we're dooooone wooo\n\n\n");

      printf("Enter an integer and base:\n");
      printf("(int >= 1 and 2 <= base <= 36 or we quit): >");
      scanf("%d %d", &baseTenValue, &base);

  }







  return 0;
}
