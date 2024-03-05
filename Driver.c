#include <ntddk.h>

VOID ExampleUnloadDriver(PDRIVER_OBJECT DriverObject);
VOID ExampleDpcRoutine(PKDPC Dpc, PVOID DeferredContext, PVOID SystemArgument1, PVOID SystemArgument2);
VOID ExampleTimerRoutine(PKDPC Dpc, PVOID DeferredContext, PVOID SystemArgument1, PVOID SystemArgument2);

KDPC gTimerDpc;
KTIMER gTimer;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = ExampleUnloadDriver;

    KeInitializeDpc(&gTimerDpc, ExampleTimerRoutine, NULL);
    KeInitializeTimer(&gTimer);
    LARGE_INTEGER dueTime;
    dueTime.QuadPart = -10000 * 1; 
    KeSetTimerEx(&gTimer, dueTime, 1, &gTimerDpc);

    DbgPrint("Example Driver Loaded\n");
    return STATUS_SUCCESS;
}

VOID ExampleTimerRoutine(PKDPC Dpc, PVOID DeferredContext, PVOID SystemArgument1, PVOID SystemArgument2) {
    UNREFERENCED_PARAMETER(Dpc);
    UNREFERENCED_PARAMETER(DeferredContext);
    UNREFERENCED_PARAMETER(SystemArgument1);
    UNREFERENCED_PARAMETER(SystemArgument2);

    PKDPC workloadDpc = ExAllocatePool(NonPagedPool, sizeof(KDPC));
    if (workloadDpc != NULL) {
        KeInitializeDpc(workloadDpc, ExampleDpcRoutine, NULL);
        KeInsertQueueDpc(workloadDpc, NULL, NULL);
    }
}

VOID ExampleDpcRoutine(PKDPC Dpc, PVOID DeferredContext, PVOID SystemArgument1, PVOID SystemArgument2) {
    UNREFERENCED_PARAMETER(DeferredContext);
    UNREFERENCED_PARAMETER(SystemArgument1);
    UNREFERENCED_PARAMETER(SystemArgument2);

    volatile ULONG64 accumulator = 0;
    for (ULONG i = 0; i < 700000; i++) { // Adjust the workload here
        accumulator += i * i;
    }

    DbgPrint("DPC Routine Executed, Accumulator: %llu\n", accumulator);
    ExFreePool(Dpc);
}


VOID ExampleUnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    KeCancelTimer(&gTimer);
    DbgPrint("Example Driver Unloading\n");
}
