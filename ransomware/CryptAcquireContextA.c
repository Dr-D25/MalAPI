/*CryptAcquireContextA используется для получения дескриптора определенного контейнера
ключей в пределах определенного поставщика криптографических услуг (CSP).*/

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>

typedef BOOL (WINAPI *PFN_CryptAcquireContextA)(
    HCRYPTPROV *phProv,
    LPCSTR pszContainer,
    LPCSTR pszProvider,
    DWORD dwProvType,
    DWORD dwFlags
);

typedef BOOL (WINAPI *PFN_CryptReleaseContext)(HCRYPTPROV hProv, DWORD dwFlags);

int main(void) {
    HMODULE hAdvapi = LoadLibraryA("Advapi32.dll");
    if (!hAdvapi) {
        printf("LoadLibraryA failed. Error = %lu\n", GetLastError());
        return 1;
    }

    PFN_CryptAcquireContextA pCryptAcquireContextA =
        (PFN_CryptAcquireContextA)GetProcAddress(hAdvapi, "CryptAcquireContextA");

    PFN_CryptReleaseContext pCryptReleaseContext =
        (PFN_CryptReleaseContext)GetProcAddress(hAdvapi, "CryptReleaseContext");

    if (!pCryptAcquireContextA || !pCryptReleaseContext) {
        printf("GetProcAddress failed. Error = %lu\n", GetLastError());
        FreeLibrary(hAdvapi);
        return 1;
    }

    HCRYPTPROV hProv = 0;

    if (!pCryptAcquireContextA(
            &hProv,
            NULL,
            NULL,
            PROV_RSA_FULL,
            CRYPT_VERIFYCONTEXT))
    {
        printf("CryptAcquireContextA (dynamic) failed. Error = %lu\n", GetLastError());
        FreeLibrary(hAdvapi);
        return 1;
    }

    printf("CryptAcquireContextA succeeded dynamically! Handle = 0x%p\n", (void*)hProv);

    pCryptReleaseContext(hProv, 0);
    FreeLibrary(hAdvapi);
    return 0;

    exit(0);
}
