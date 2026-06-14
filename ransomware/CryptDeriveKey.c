/*CryptDeriveKey используется для создания ключей шифрования.*/

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

typedef BOOL (WINAPI *PFN_CryptCreateHash)(
    HCRYPTPROV hProv,
    ALG_ID Algid,
    HCRYPTKEY hKey,
    DWORD dwFlags,
    HCRYPTHASH *phHash
);

typedef BOOL (WINAPI *PFN_CryptHashData)(
    HCRYPTHASH hHash,
    const BYTE *pbData,
    DWORD dwDataLen,
    DWORD dwFlags
);

typedef BOOL (WINAPI *PFN_CryptDeriveKey)(
    HCRYPTPROV hProv,
    ALG_ID Algid,
    HCRYPTHASH hBaseData,
    DWORD dwFlags,
    HCRYPTKEY *phKey
);

typedef BOOL (WINAPI *PFN_CryptDestroyHash)(
    HCRYPTHASH hHash
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
    PFN_CryptCreateHash pCryptCreateHash =
        (PFN_CryptCreateHash)GetProcAddress(hAdvapi, "CryptCreateHash");
    PFN_CryptHashData pCryptHashData =
        (PFN_CryptHashData)GetProcAddress(hAdvapi, "CryptHashData");
    PFN_CryptDeriveKey pCryptDeriveKey =
        (PFN_CryptDeriveKey)GetProcAddress(hAdvapi, "CryptDeriveKey");
    PFN_CryptDestroyHash pCryptDestroyHash =
        (PFN_CryptDestroyHash)GetProcAddress(hAdvapi, "CryptDestroyHash");
    PFN_CryptDestroyKey pCryptDestroyKey =
        (PFN_CryptDestroyKey)GetProcAddress(hAdvapi, "CryptDestroyKey");
    PFN_CryptReleaseContext pCryptReleaseContext =
        (PFN_CryptReleaseContext)GetProcAddress(hAdvapi, "CryptReleaseContext");

    if (!pCryptAcquireContextA || !pCryptCreateHash || !pCryptHashData ||
        !pCryptDeriveKey || !pCryptDestroyHash || !pCryptDestroyKey ||
        !pCryptReleaseContext)
    {
        printf("GetProcAddress failed.\n");
        return 1;
    }

    HCRYPTPROV hProv = 0;
    if (!pCryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        printf("CryptAcquireContextA failed. Error = %lu\n", GetLastError());
        return 1;
    }

    HCRYPTHASH hHash = 0;
    if (!pCryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        printf("CryptCreateHash failed. Error = %lu\n", GetLastError());
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    const char *password = "SecretPassword123567890Admin";

    if (!pCryptHashData(hHash, (BYTE*)password, (DWORD)strlen(password), 0)) {
        printf("CryptHashData failed. Error = %lu\n", GetLastError());
        pCryptDestroyHash(hHash);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    HCRYPTKEY hKey = 0;
    if (!pCryptDeriveKey(hProv, CALG_AES_128, hHash, 0, &hKey)) {
        printf("CryptDeriveKey failed. Error = %lu\n", GetLastError());
        pCryptDestroyHash(hHash);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    printf("AES-128 key derived from password successfully!\n");

    pCryptDestroyKey(hKey);
    pCryptDestroyHash(hHash);
    pCryptReleaseContext(hProv, 0);
    FreeLibrary(hAdvapi);

    return 0;
    exit(0);
}
