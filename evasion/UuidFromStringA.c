#include <windows.h>
#include <rpc.h>
#include <stdio.h>

#pragma comment(lib, "Rpcrt4.lib")

int main() {
    UUID uuid;
    RPC_CSTR stringUuid = (RPC_CSTR)"550e8400-e29b-41d4-a716-446655440000";

    RPC_STATUS status = UuidFromStringA(stringUuid, &uuid);

    if (status == RPC_S_OK) {
        printf("UUID done. \n");
        printf("Data1: %08x\n", uuid.Data1);
        printf("Data2: %04x\n", uuid.Data2);
        printf("Data3: %04x\n", uuid.Data3);
        printf("Data4: ");
        for (int i = 0; i < 8; i++) {
            printf("%02x", uuid.Data4[i]);
        }
        printf("\n");
    } else {
        printf("UuidFromStringA failed with error: %lu\n", status);
    }

    return 0;
}
