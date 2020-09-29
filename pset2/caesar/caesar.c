# include <stdio.h>
# include <cs50.h>
# include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2) // making sure there is a second comand arg
    {
        printf("Usage ./ceasar key\n");
        return 1;
    }

    string text = argv[1];
    int text1;

    int len = strlen(text);
    for (int i = 0; i <= len; i++) // going through comand line args
    {
        if (text[i] >= (char) 48 && text[i] <= (char) 57) // saving comand arg's given value
        {
            text1 = text1 * 10 + (int) text[i] - 48;
        }
        if (text[i] > (char) 57) // making sure second comand arg is only numeric
        {
            printf("Usage ./ceasar key\n");
            return 1;
        }
    }

    string p_text = get_string("plaintext: "); // prompting for a string
    int l_msg = strlen(p_text);
    string c_text = p_text;
    string b_text = p_text;
    char c;
    char a;

    int t = 0;
    for (int h = text1; h > 24; h -= 25)
    {
        t++;
    }

    int h = text1;
    if (text1 > 24)
    {
        h = text1 - 25 * t;
    }
    int text2 = 26 - h;

    for (int j = 0; j <= l_msg; j++)
    {
        if (p_text[j] >= 'A' && p_text[j <= 'Z']) // going through string given
        {
            a = (char) p_text[j] - text2;
            c_text[j] = (char) p_text[j] + text1; // adding value to chars in string to cipher it

            if (c_text[j] > 'z' || c_text[j] < 0)
            {
                b_text[j] = a;
            }
            if (c_text[j] < 'z')
            {
                b_text[j] = c_text[j];
            }
        }
        c = (char) p_text[0] + text1; // in case string is 1 char
    }

    if (l_msg > 1) //printing new string
    {
        printf("ciphertext: %s\n", b_text);
    }
    if (l_msg == 1) // printing the char
    {
        printf("ciphertext: %c\n", c);
    }
}