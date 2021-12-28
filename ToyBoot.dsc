[Defines]
    PLATFORM_NAME = ToyBootPkg
    PLATFORM_GUID = fbe90256-121a-4da8-a722-144c4d87d2cb
    PLATFORM_VERSION = 0.0.1
    DSC_SPECIFICATION = 0x00010005
    BUILD_TARGETS = DEBUG|RELEASE
    SUPPORTED_ARCHITECTURES = X64
    OUTPUT_DIRECTORY = ToyBootPkg/Build

[LibraryClasses]
    UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
    PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
    UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
    PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
    MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
    DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
    BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
    BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
    UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
    DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
    RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
    DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
    UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

[Components]
    ToyBootPkg/ToyBootPkg.inf
