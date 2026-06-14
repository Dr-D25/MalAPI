/*CryptGenRandom используется для заполнения буфера криптографически случайными байтами.*/

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>

typedef BOOL (WINAPI *PFN_CryptGenRandom)(
    HCRYPTPROV hProv,
    DWORD dwLen,
    BYTE *pbBuffer
);

typedef BOOL (WINAPI *PFN_CryptAcquireContextA)(
    HCRYPTPROV *phProv,
    LPCSTR pszContainer,
    LPCSTR pszProvider,
    DWORD dwProvType,
    DWORD dwFlags
);

typedef BOOL (WINAPI *PFN_CryptReleaseContext)(HCRYPTPROV hProv, DWORD dwFlags);

int main(void)
{
    HMODULE hAdvapi = LoadLibraryA("Advapi32.dll");
    if (!hAdvapi) {
        printf("LoadLibrary failed. Error = %lu\n", GetLastError());
        return 1;
    }

    PFN_CryptAcquireContextA pCryptAcquireContextA =
        (PFN_CryptAcquireContextA)GetProcAddress(hAdvapi, "CryptAcquireContextA");
    PFN_CryptGenRandom pCryptGenRandom =
        (PFN_CryptGenRandom)GetProcAddress(hAdvapi, "CryptGenRandom");
    PFN_CryptReleaseContext pCryptReleaseContext =
        (PFN_CryptReleaseContext)GetProcAddress(hAdvapi, "CryptReleaseContext");

    if (!pCryptAcquireContextA || !pCryptGenRandom || !pCryptReleaseContext) {
        printf("GetProcAddress failed. Error = %lu\n", GetLastError());
        return 1;
    }

    HCRYPTPROV hProv = 0;
    if (!pCryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        printf("CryptAcquireContextA failed. Error = %lu\n", GetLastError());
        return 1;
    }

    BYTE buffer[512];

    if (!pCryptGenRandom(hProv, sizeof(buffer), buffer)) {
        printf("CryptGenRandom failed. Error = %lu\n", GetLastError());
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    printf("Random bytes: ");
    for (int i = 0; i < sizeof(buffer); i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    pCryptReleaseContext(hProv, 0);
    FreeLibrary(hAdvapi);

    return 0;
    exit(0);
}
