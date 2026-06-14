/*CryptDecrypt используется для расшифровки данных.*/

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>

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

typedef BOOL (WINAPI *PFN_CryptEncrypt)(
    HCRYPTKEY hKey,
    HCRYPTHASH hHash,
    BOOL Final,
    DWORD dwFlags,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwBufLen
);

typedef BOOL (WINAPI *PFN_CryptDecrypt)(
    HCRYPTKEY hKey,
    HCRYPTHASH hHash,
    BOOL Final,
    DWORD dwFlags,
    BYTE *pbData,
    DWORD *pdwDataLen
);

typedef BOOL (WINAPI *PFN_CryptDestroyKey)(
    HCRYPTKEY hKey
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
    PFN_CryptGenKey pCryptGenKey =
        (PFN_CryptGenKey)GetProcAddress(hAdvapi, "CryptGenKey");
    PFN_CryptEncrypt pCryptEncrypt =
        (PFN_CryptEncrypt)GetProcAddress(hAdvapi, "CryptEncrypt");
    PFN_CryptDecrypt pCryptDecrypt =
        (PFN_CryptDecrypt)GetProcAddress(hAdvapi, "CryptDecrypt");
    PFN_CryptDestroyKey pCryptDestroyKey =
        (PFN_CryptDestroyKey)GetProcAddress(hAdvapi, "CryptDestroyKey");
    PFN_CryptReleaseContext pCryptReleaseContext =
        (PFN_CryptReleaseContext)GetProcAddress(hAdvapi, "CryptReleaseContext");

    if (!pCryptAcquireContextA || !pCryptGenKey || !pCryptEncrypt ||
        !pCryptDecrypt || !pCryptDestroyKey || !pCryptReleaseContext)
    {
        printf("GetProcAddress failed. Error = %lu\n", GetLastError());
        return 1;
    }

    HCRYPTPROV hProv = 0;
    if (!pCryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        printf("CryptAcquireContext failed. Error = %lu\n", GetLastError());
        return 1;
    }

    HCRYPTKEY hKey = 0;
    if (!pCryptGenKey(hProv, CALG_RC4, 0, &hKey)) {
        printf("CryptGenKey failed. Error = %lu\n", GetLastError());
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    BYTE data[128] = "Rev0x1337";
    DWORD dataLen = (DWORD)strlen((char*)data);
    DWORD bufLen = sizeof(data);

    printf("Original text: %s\n", data);

    if (!pCryptEncrypt(hKey, 0, TRUE, 0, data, &dataLen, bufLen)) {
        printf("CryptEncrypt failed. Error = %lu\n", GetLastError());
        pCryptDestroyKey(hKey);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    printf("Encrypted (hex): ");
    for (DWORD i = 0; i < dataLen; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    if (!pCryptDecrypt(hKey, 0, TRUE, 0, data, &dataLen)) {
        printf("CryptDecrypt failed. Error = %lu\n", GetLastError());
        pCryptDestroyKey(hKey);
        pCryptReleaseContext(hProv, 0);
        return 1;
    }

    data[dataLen] = 0;
    printf("Decrypted text: %s\n", data);

    pCryptDestroyKey(hKey);
    pCryptReleaseContext(hProv, 0);
    FreeLibrary(hAdvapi);

    return 0;
    exit(0);
}
