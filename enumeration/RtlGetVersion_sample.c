#include <ntddk.h>


typedef struct _OSVERSIONINFOEXW {
    ULONG dwOSVersionInfoSize;
    ULONG dwMajorVersion;
    ULONG dwMinorVersion;
    ULONG dwBuildNumber;
    ULONG dwPlatformId;
    WCHAR szCSDVersion[128];
    USHORT wServicePackMajor;
    USHORT wServicePackMinor;
    USHORT wSuiteMask;
    UCHAR wProductType;
    UCHAR wReserved;
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW;


NTSTATUS NTAPI RtlGetVersion(POSVERSIONINFOEXW VersionInformation);


DRIVER_INITIALIZE DriverEntry;


NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS Status = STATUS_SUCCESS;
    OSVERSIONINFOEXW osvi;


    memset(&osvi, 0, sizeof(OSVERSIONINFOEXW));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);


    Status = RtlGetVersion(&osvi);

    if (!NT_SUCCESS(Status)) {
        DbgPrint("Error call RtlGetVersion\n");
        return Status;
    }


    DbgPrint("Version OS:\n");
    DbgPrint("Major version: %lu\n", osvi.dwMajorVersion);
    DbgPrint("Minor version: %lu\n", osvi.dwMinorVersion);
    DbgPrint("Build number: %lu\n", osvi.dwBuildNumber);
    DbgPrint("Service Pack Major: %hu\n", osvi.wServicePackMajor);
    DbgPrint("Service Pack Minor: %hu\n", osvi.wServicePackMinor);

    return Status;
}
