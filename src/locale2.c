#include <locale.h>
#include <wchar.h>
#include <stdio.h>

int main() {
    // Set the locale to the user's default locale
    setlocale(LC_ALL, "");  
//print the locale
	wprintf(L"Locale is: %s\n", setlocale(LC_ALL, ""));
    for (int i = 128; i <= 255; i++) {
        wchar_t c = (wchar_t)i;
		//starts printing anything recognizable from the 161th character
        wprintf(L"%lc ", c);
    }
    wprintf(L"\n");
    return 0;
}