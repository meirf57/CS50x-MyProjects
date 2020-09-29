# include <stdio.h>

// Prompting for name.. saying hello
int main(void)

{
    char *name;
    printf("What's you'r name: ");
    scanf("%s", &name); 
    printf("Hello %s\n", &name);
    return 0;
}
