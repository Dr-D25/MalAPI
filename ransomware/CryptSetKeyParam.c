/*CryptSetKeyParam используется для настройки различных аспектов работы сеансового ключа.*/

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

typedef BOOL (WINAPI *PFN_CryptGenKey)(
    HCRYPTPROV hProv,
    ALG_ID Algid,
    DWORD dwFlags,
    HCRYPTKEY *phKey
);

typedef BOOL (WINAPI *PFN_CryptSetKeyParam)(
    HCRYPTKEY hKey,
    DWORD dwParam,
    const BYTE *pbData,
    DWORD dwFlags
);

typedef BOOL (WINAPI *PFN_CryptDestroyKey)(
    HCRYPTKEY hKey
);

typedef BOOL (WINAPI *PFN_CryptReleaseContext)(
    HCRYPTPROV hProv,
    DWORD dwFlags
);

int main(void)
{
    HMODULE hAdvapi = LoadLibraryA("Advapi32.dll");
    if (!hAdvapi) {
        printf("LoadLibrary failed. Error = %lu\n", GetLastError());
        return 1;
    }

    PFN_CryptAcquireContextA pCryptAcquireContextA =
        (PFN_CryptAcquireContextA)GetProcAddress(hAdvapi, "CryptAcquireContextA");

    PFN_CryptGenKey pCryptGenKey =
        (PFN_CryptGenKey)GetProcAddress(hAdvapi, "CryptGenKey");

    PFN_CryptSetKeyParam pCryptSetKeyParam =
        (PFN_CryptSetKeyParam)GetProcAddress(hAdvapi, "CryptSetKeyParam");

    PFN_CryptDestroyKey pCryptDestroyKey =
        (PFN_CryptDestroyKey)GetProcAddress(hAdvapi, "CryptDestroyKey");

    PFN_CryptReleaseContext pCryptReleaseContext =
        (PFN_CryptReleaseContext)GetProcAddress(hAdvapi, "CryptReleaseContext");

    if (!pCryptAcquireContextA || !pCryptGenKey ||
        !pCryptSetKeyParam || !pCryptDestroyKey || !pCryptReleaseContext)
    {
        printf("GetProcAddress failed!\n");
        return 1;
    }

    HCRYPTPROV hProv = 0;
    if (!pCryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        printf("CryptAcquireContextA failed. Error = %lu\n", GetLastError());
        return 1;
    }

    HCRYPTKEY hKey = 0;
    if (!pCryptGenKey(hProv, CALG_AES_128, CRYPT_EXPORTABLE, &hKey)) {
        printf("CryptGenKey failed. Error = %lu\n", GetLastError());
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    printf("AES-128 key generated!\n");

    BYTE iv[16] = {
        0x01,0x02,0x03,0x04, 0x05,0x06,0x07,0x08,
        0x09,0x0A,0x0B,0x0C, 0x0D,0x0E,0x0F,0x10
    };

    if (!pCryptSetKeyParam(hKey, KP_IV, iv, 0)) {
        printf("CryptSetKeyParam (KP_IV) failed. Error = %lu\n", GetLastError());
        pCryptDestroyKey(hKey);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    printf("IV successfully set via CryptSetKeyParam!\n");

    pCryptDestroyKey(hKey);
    pCryptReleaseContext(hProv, 0);
    FreeLibrary(hAdvapi);

    return 0;
    exit(0);
}
