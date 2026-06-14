/*CryptCreateHash используется для создания хеша.*/

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

typedef BOOL (WINAPI *PFN_CryptGetHashParam)(
    HCRYPTHASH hHash,
    DWORD dwParam,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwFlags
);

typedef BOOL (WINAPI *PFN_CryptDestroyHash)(
    HCRYPTHASH hHash
);

typedef BOOL (WINAPI *PFN_CryptReleaseContext)(
    HCRYPTPROV hProv,
    DWORD dwFlags
);

int main(void) {
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
    PFN_CryptGetHashParam pCryptGetHashParam =
        (PFN_CryptGetHashParam)GetProcAddress(hAdvapi, "CryptGetHashParam");
    PFN_CryptDestroyHash pCryptDestroyHash =
        (PFN_CryptDestroyHash)GetProcAddress(hAdvapi, "CryptDestroyHash");
    PFN_CryptReleaseContext pCryptReleaseContext =
        (PFN_CryptReleaseContext)GetProcAddress(hAdvapi, "CryptReleaseContext");

    if (!pCryptAcquireContextA || !pCryptCreateHash || !pCryptHashData ||
        !pCryptGetHashParam || !pCryptDestroyHash || !pCryptReleaseContext)
    {
        printf("GetProcAddress failed. Error = %lu\n", GetLastError());
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

    const char *text = "REV0x1337";
    if (!pCryptHashData(hHash, (BYTE*)text, (DWORD)strlen(text), 0)) {
        printf("CryptHashData failed. Error = %lu\n", GetLastError());
        pCryptDestroyHash(hHash);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    BYTE hash[32];
    DWORD hashLen = sizeof(hash);

    if (!pCryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        printf("CryptGetHashParam failed. Error = %lu\n", GetLastError());
        pCryptDestroyHash(hHash);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    printf("SHA-256 hash: ");
    for (DWORD i = 0; i < hashLen; i++) {
        printf("%02X", hash[i]);
    }
    printf("\n");

    pCryptDestroyHash(hHash);
    pCryptReleaseContext(hProv, 0);
    FreeLibrary(hAdvapi);

    return 0;
    exit(0);
}
