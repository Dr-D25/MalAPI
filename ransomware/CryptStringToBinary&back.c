/*Функция CryptBinaryToString используется для преобразования массива байтов в форматированную строку и обратно.*/

#include <windows.h>
#include <stdio.h>

typedef BOOL (WINAPI *LPCRYPTSTRINGTOBINARYW)(
    LPCWSTR pszString,
    DWORD cchString,
    DWORD dwFlags,
    BYTE *pbBinary,
    DWORD *pcbBinary,
    DWORD *pdwSkip,
    DWORD *pdwFlags
);

typedef BOOL (WINAPI *LPCRYPTBINARYTOSTRINGW)(
    const BYTE *pbBinary,
    DWORD cbBinary,
    DWORD dwFlags,
    LPWSTR pszString,
    DWORD *pcchString
);

int main() {
    HMODULE hCrypt = LoadLibraryA("Crypt32.dll");
    if (!hCrypt) {
        printf("Failed to load Crypt32.dll\n");
        return 1;
    }

    LPCRYPTSTRINGTOBINARYW pCryptStringToBinaryW =
        (LPCRYPTSTRINGTOBINARYW)GetProcAddress(hCrypt, "CryptStringToBinaryW");
    LPCRYPTBINARYTOSTRINGW pCryptBinaryToStringW =
        (LPCRYPTBINARYTOSTRINGW)GetProcAddress(hCrypt, "CryptBinaryToStringW");

    if (!pCryptStringToBinaryW || !pCryptBinaryToStringW) {
        printf("Failed to get function addresses\n");
        FreeLibrary(hCrypt);
        return 1;
    }

    LPCWSTR base64 = L"SGVsbG8sIFdvcmxkIQ==";  // "Hello, World!"

    DWORD binarySize = 0;

    pCryptStringToBinaryW(base64, 0, CRYPT_STRING_BASE64, NULL, &binarySize, NULL, NULL);

    BYTE *binary = (BYTE*)malloc(binarySize);
    if (!binary) return 1;

    if (!pCryptStringToBinaryW(base64, 0, CRYPT_STRING_BASE64, binary, &binarySize, NULL, NULL)) {
        printf("Error CryptStringToBinaryW\n");
        free(binary);
        FreeLibrary(hCrypt);
        return 1;
    }

    printf("Base64 - Binary successfully. Size: %lu byte\n", binarySize);

    DWORD outSize = 0;
    pCryptBinaryToStringW(binary, binarySize, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR, NULL, &outSize);

    WCHAR *outString = (WCHAR*)malloc(outSize * sizeof(WCHAR));
    if (!outString) {
        free(binary);
        return 1;
    }

    if (!pCryptBinaryToStringW(binary, binarySize, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR,
                               outString, &outSize)) {
        printf("Error CryptBinaryToStringW\n");
        free(binary);
        free(outString);
        FreeLibrary(hCrypt);
        return 1;
    }

    wprintf(L"Binary - Base64: %s\n", outString);

    free(binary);
    free(outString);
    FreeLibrary(hCrypt);

    return 0;
}
