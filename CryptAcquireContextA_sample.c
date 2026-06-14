#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>

#pragma comment(lib,"advapi32.lib")

int main() {
    HCRYPTPROV hProv;
    BOOL retVal;


    LPCSTR providerName = MS_ENH_RSA_AES_PROV_A;


    retVal = CryptAcquireContextA(&hProv, NULL, providerName, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);

    if(retVal) {
        printf("CryptContext create.\n");
    } else {
        DWORD lastError = GetLastError();
        printf("Error create CryptContext. Code error: %lu\n", lastError);
        return 1;
    }


    CryptReleaseContext(hProv, 0);

    return 0;
}
