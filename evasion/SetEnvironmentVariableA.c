#include <windows.h>
#include <stdio.h>

int main() {
    const char* varName = "MY_TEST_VARIABLE";
    const char* varValue = "HelloWorld";

    if (SetEnvironmentVariableA(varName, varValue)) {
        printf("Environment variable '%s' successfully set to'%s'\n", varName, varValue);
    } else {
        printf("Failed to set environment variable. Error: %lu\n", GetLastError());
    }

    char buffer[256];
    DWORD ret = GetEnvironmentVariableA(varName, buffer, sizeof(buffer));
    if (ret > 0 && ret < sizeof(buffer)) {
        printf("Variable value'%s': %s\n", varName, buffer);
    } else {
        printf("Failed to get variable value'%s'.Error: %lu\n", varName, GetLastError());
    }

    return 0;
}
