#include <locale.h>
#include <wchar.h>
#include <stdio.h>

int main() {
	// Set the locale to the user's default locale
    setlocale(LC_ALL, "");  
	
	// wchat_t is a wide character type
    wchar_t c1 = L'ò';
	wchar_t c2 = L'¡';
	  
	// %lc is the format specifier for a wide character  
    wprintf(L"%lc %lc\n", c1, c2);
    return 0;
}