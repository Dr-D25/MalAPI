#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>

#pragma comment(lib, "Crypt32.lib")

int main() {
    DATA_BLOB DataIn;
    DATA_BLOB DataOut;
    DATA_BLOB DataEntropy;
    BOOL result;

    const char* secret = "Sensitive data to protect";
    DataIn.pbData = (BYTE*)secret;
    DataIn.cbData = (DWORD)strlen(secret);

    DataEntropy.pbData = NULL;
    DataEntropy.cbData = 0;

    result = CryptProtectData(
        &DataIn,            // данные для шифрования
        L"Description",     // описание (можно NULL)
        &DataEntropy,       // дополнительная энтропия
        NULL,               // reserved, должен быть NULL
        NULL,               // optional prompt struct
        0,                  // флаги (0 - стандартные)
        &DataOut            // зашифрованные данные (выход)
    );

    if (!result) {
        printf("CryptProtectData failed. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("Data encrypted successfully! Size: %lu байт\n", DataOut.cbData);


    LocalFree(DataOut.pbData);

    return 0;
}
