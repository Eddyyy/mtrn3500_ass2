#include <iostream>
 
#ifdef PACKED
struct __attribute__((__packed__)) mystruct_A
{
       char a;
          int b;
             char c;
}x;
 
#else /* !PACKED */
 
struct mystruct_A
{
       char a;
          int b;
             char c;
}x;
 
#endif /* PACKED */
 
int main()
{
            std::cout<<sizeof(x)<<'\n';
}
