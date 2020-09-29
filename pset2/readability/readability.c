# include <stdio.h>
# include <cs50.h>
# include <string.h>
# include <math.h>

int main(void)
{
    string text = get_string("Text: ");

    int len = strlen(text); // length of text from input


    float ltrs = 0; // num of letters

    for (int l = 0; l <= len; l++) // counting out the letters in text
    {
        if (text[l] >= 'A' && text[l <= 'Z'])
        {
            ltrs = ltrs + 1;
        }
    }


    float word = 0; // num of words

    for (int w = 0; w <= len; w++) // counting the words in text
    {
        if (text[w] == ' ' || text[w] == '\0')
        {
            word = word + 1;
        }
    }



    float sentence = 0; // num of sentence(s)

    for (int s = 0; s <= len; s++) // counting the sentence(s) in text
    {
        if (text[s] == '.' || text[s] == '!' || text[s] == '?')
        {
            sentence = sentence + 1;
        }
    }


    // implementing the Coleman-Liau index to grade text
    float inl = (ltrs / word) * 100;
    float ins = (sentence / word) * 100;
    int cli = round((0.0588 * inl) - (0.296 * ins) - 15.8);


    // printing the grade based on CLi
    if (cli < 1)
    {
        printf("Before Grade 1\n");
    }

    if (cli >= 16)
    {
        printf("Grade 16+\n");
    }

    if (cli >= 1.0 && cli < 16)
    {
        printf("Grade %i\n", cli);
    }
}