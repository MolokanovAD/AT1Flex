%option noyywrap
%option c++
letter [A-Za-z_.]
digit [0-9]
%%
(\ )+ {return 4;}
\n {return 3;}
: {return 2;} 
[^A-Za-z_. \n]([^(\ \n)])* {return 0;}
{letter}({letter}|{digit})* {return 1;}
<<EOF>> return -1;
%%