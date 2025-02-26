%{
	int vow_count = 0;
	int const_count = 0;
%}

%%

[aeiouAEIOU]   { vow_count++; }
[a-zA-Z]       { const_count++; }
\n             { return 0; } /* Stop processing on a newline */
.              ; /* Ignore other characters */

%%

int yywrap() { return 1; }

int main()
{
	printf("Enter a string of vowels and consonants: ");
	yylex(); // Process the input
	printf("Number of vowels: %d\n", vow_count);
	printf("Number of consonants: %d\n", const_count);
	return 0;
}
