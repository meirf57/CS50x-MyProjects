# include <stdio.h>
# include <cs50.h>
# include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage ./ceasar key\n");
        return 1;
    }

    string text = argv[1];
    int text1;

    int len = strlen(text);
    for (int i = 0; i <= len; i++)
    {
        if (text[i] >= (char) 48 && text[i] <= (char) 57)
        {
            text1 = text1 * 10 + (int) text[i] - 48;
        }
        if (text[i] > (char) 57)
        {
            printf("Usage ./ceasar key\n");
            return 1;
        }
    }


    string p_text = get_string("plaintext: ");
    int l_msg = strlen(p_text);
    string c_text = p_text;
    
    
    for (int j = 0; j <= l_msg; j++)
    {
        if (p_text[j] >= 'A' && p_text[j <= 'Z'])
        c_text[j] = (char) p_text[j] + text1;
    }

    printf("ciphertext: %s\n", c_text);

}