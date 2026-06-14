#include <windows.h>
#include <stdio.h>


void print_context(const CONTEXT* context)
{
    printf("EAX = %x\n", context->Rax);
    printf("EBX = %x\n", context->Rbx);
    printf("ECX = %x\n", context->Rcx);
    printf("EDX = %x\n", context->Rdx);
    printf("ESP = %x\n", context->Rsp);
    printf("EBP = %x\n", context->Rbp);
}

int main()
{
    HANDLE hThread = GetCurrentThread();

    if (!hThread)
    {
        fprintf(stderr, "Error getting stream descriptor.\n");
        return 1;
    }

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;

    BOOL result = GetThreadContext(hThread, &ctx);

    if (!result)
    {
        DWORD errorCode = GetLastError();
        fprintf(stderr, "Error calling GetThreadContext: error code %lu\n", errorCode);
        CloseHandle(hThread);
        return 1;
    }

    print_context(&ctx);

    CloseHandle(hThread);

    return 0;
}
