#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#include <Log.h>


#define CHECK_ERROR_BEFORELOG(s,message) LogStatus(s,message)
#define RETRURN_IF_ERROR(s,message) EFI_ERROR (CHECK_ERROR_BEFORELOG(s,message))

#define MAX_BUFFER_SIZE 4*1024
#define LOG 1


typedef EFI_PHYSICAL_ADDRESS address;

typedef EFI_PHYSICAL_ADDRESS ADDRESS;



