#include <windows.h>
#include <stdio.h>


void print_drive_type(const char* path)
{
    DWORD drive_type = GetDriveTypeA(path);

    switch(drive_type)
    {
        case DRIVE_UNKNOWN:
            printf("Unknown type.\n");
            break;

        case DRIVE_NO_ROOT_DIR:
            printf("There is no root directory.\n");
            break;

        case DRIVE_REMOVABLE:
            printf("Removable disk.\n");
            break;

        case DRIVE_FIXED:
            printf("Hard disk.\n");
            break;

        case DRIVE_REMOTE:
            printf("Remote Network Drive.\n");
            break;

        case DRIVE_CDROM:
            printf("CD/DVD.\n");
            break;

        case DRIVE_RAMDISK:
            printf("RAM-disk.\n");
            break;

        default:
            printf("Unknown error determining device type.\n");
            break;
    }
}

int main()
{

    const char *path = "C:\\";
    print_drive_type(path);

    return 0;
}
