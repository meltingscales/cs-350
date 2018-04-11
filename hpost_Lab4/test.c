int main()
{
  printf("ass\n");

  int *b[] = {2,4,2,8};

  int *p,*q,*r;

  p = &b;
  q = &b[1];
  r = &b[2];

  b[0] = 420; //look, changes are saved. wow. holy shit.

  printf("wow %d\n\n",*p);

  b[0] = 2;

  printf("p = %d, q = %d, r = %d.\n\n",*p,*q,*r);

  printf("p<q<r is %s\n",(p<q<r)?"true":"false");
  printf("%d < %d < %d\n",p,q,r);

  return 0;
}
