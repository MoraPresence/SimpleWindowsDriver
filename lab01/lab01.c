#include <ntddk.h>

#define THE_BUFFER_LENGTH 16
#define FILE_DEVICE_IOCTL  0x00008301
#define IOCTL_MY_NEITHER   CTL_CODE(FILE_DEVICE_IOCTL, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_MY_BUFFERED  CTL_CODE(FILE_DEVICE_IOCTL, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MY_INDIRECT  CTL_CODE(FILE_DEVICE_IOCTL, 0x802, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_MY_OUTDIRECT CTL_CODE(FILE_DEVICE_IOCTL, 0x803, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

typedef struct _DEVICE_EXTENSION
{
   unsigned char Image[THE_BUFFER_LENGTH];
}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define DEV_NAME0 L"\\Device\\lab01_3_BO"
#define DEV_NAME1 L"\\Device\\lab01_3_DO"
#define DEV_NAME2 L"\\Device\\lab01_3_NO"
#define DEV_NAME3 L"\\Device\\lab01_3_O"
#define LINK_NAME0 L"\\??\\MyLink0"
#define LINK_NAME1 L"\\??\\MyLink1"
#define LINK_NAME2 L"\\??\\MyLink2"
#define LINK_NAME3 L"\\??\\MyLink3"

//-----------------------------------------------------------------------------------------------------
void OnUnload(PDRIVER_OBJECT pDrv);
NTSTATUS OnCreate(PDEVICE_OBJECT pDev, PIRP pIrp);
NTSTATUS OnClose(PDEVICE_OBJECT pDev, PIRP pIrp);
NTSTATUS DriverEntry(PDRIVER_OBJECT pDrv, PUNICODE_STRING pRegPath);
NTSTATUS OnRead(PDEVICE_OBJECT pDev, PIRP pIrp);
NTSTATUS OnWrite(PDEVICE_OBJECT pDev, PIRP pIrp);
NTSTATUS OnControl(PDEVICE_OBJECT pDev, PIRP pIrp);
//-----------------------------------------------------------------------------------------------------

void OnUnload(PDRIVER_OBJECT pDrv){
	UNICODE_STRING LinkName0;
	UNICODE_STRING LinkName1;
	UNICODE_STRING LinkName2;
	UNICODE_STRING LinkName3;
	
	DbgPrint("MYDRV: Start OnUnload\n");

	RtlInitUnicodeString(&LinkName0, LINK_NAME0);
	RtlInitUnicodeString(&LinkName1, LINK_NAME1);
	RtlInitUnicodeString(&LinkName2, LINK_NAME2);
	RtlInitUnicodeString(&LinkName3, LINK_NAME3);
	
//5. Delete device
	while(pDrv->DeviceObject)
	IoDeleteDevice(pDrv->DeviceObject);
//6. Delete Symbolic Link
	IoDeleteSymbolicLink(&LinkName0);
	IoDeleteSymbolicLink(&LinkName1);
	IoDeleteSymbolicLink(&LinkName2);
	IoDeleteSymbolicLink(&LinkName3);

	DbgPrint("MYDRV: Success OnUnload\n");
}

//2. Realization obrabotchiki request IRP_MJ_CREATE, IRP_MJ_CLOSE
NTSTATUS OnCreate(PDEVICE_OBJECT pDev, PIRP pIrp){
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	DbgPrint("MYDRV: OnCreate\n");

	if(pStack->FileObject && pStack->FileObject->FileName.Buffer)
		DbgPrint("Name is <%ws>\n", pStack->FileObject->FileName.Buffer);

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, 0); //IoMarkIrpPending(Irp)
	return STATUS_SUCCESS;
}

NTSTATUS OnClose(PDEVICE_OBJECT pDev, PIRP pIrp){
	DbgPrint("MYDRV: OnClose\n");
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrv, PUNICODE_STRING pRegPath){
	UNICODE_STRING DevName0, LinkName0;
	UNICODE_STRING DevName1, LinkName1;
	UNICODE_STRING DevName2, LinkName2;
	UNICODE_STRING DevName3, LinkName3;
	PDEVICE_OBJECT pDev;
	NTSTATUS status;
	DbgPrint("MYDRV: DriverEntry, regpath=%ws\n", pRegPath->Buffer);
//1. Register obrabotchiki requests IRP_MJ_CREATE, IRP_MJ_CLODE
	pDrv->DriverUnload = OnUnload;
	pDrv->MajorFunction[IRP_MJ_CREATE] = OnCreate;
	pDrv->MajorFunction[IRP_MJ_CLOSE] = OnClose;
	pDrv->MajorFunction[IRP_MJ_READ] = OnRead;
	pDrv->MajorFunction[IRP_MJ_WRITE] = OnWrite;
	pDrv->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OnControl;
//3. Create device
	//3.1 Create DeviceName
	RtlInitUnicodeString(&DevName0, DEV_NAME0);
	RtlInitUnicodeString(&DevName1, DEV_NAME1);
	RtlInitUnicodeString(&DevName2, DEV_NAME2);
	RtlInitUnicodeString(&DevName3, DEV_NAME3);
	//3.2 Create Device
	status = IoCreateDevice(pDrv, 0,&DevName0, 0x8000/*FILE_DEVICE_UNKNOWN*/, 0, FALSE, &pDev);
	pDev->Flags |=DO_BUFFERED_IO;
	if(STATUS_SUCCESS != status){
		DbgPrint("MYDRV: Err: IoCreateDevice0\n");
		return status;
	}
	status = IoCreateDevice(pDrv, 0,&DevName1, 0x8000/*FILE_DEVICE_UNKNOWN*/, 0, FALSE, &pDev);
	pDev->Flags |=DO_DIRECT_IO;
	if(STATUS_SUCCESS != status){
		DbgPrint("MYDRV: Err: IoCreateDevice1\n");
		return status;
	}
	status = IoCreateDevice(pDrv, 0,&DevName2, 0x8000/*FILE_DEVICE_UNKNOWN*/, 0, FALSE, &pDev);
	if(STATUS_SUCCESS != status){
		DbgPrint("MYDRV: Err: IoCreateDevice2\n");
		return status;
	}
	status = IoCreateDevice(pDrv, 0,&DevName3, 0x8000/*FILE_DEVICE_UNKNOWN*/, 0, FALSE, &pDev);
	if(STATUS_SUCCESS != status){
		DbgPrint("MYDRV: Err: IoCreateDevice3\n");
		return status;
	}
//4. Create Symbilic Link
	RtlInitUnicodeString(&LinkName0, LINK_NAME0);
	IoCreateSymbolicLink(&LinkName0, &DevName0);
	RtlInitUnicodeString(&LinkName1, LINK_NAME1);
	IoCreateSymbolicLink(&LinkName1, &DevName1);
	RtlInitUnicodeString(&LinkName2, LINK_NAME2);
	IoCreateSymbolicLink(&LinkName2, &DevName2);
	RtlInitUnicodeString(&LinkName3, LINK_NAME3);
	IoCreateSymbolicLink(&LinkName3, &DevName3);

	DbgPrint("MYDRV: DriverEntry success");
	return STATUS_SUCCESS;
}

NTSTATUS OnRead(PDEVICE_OBJECT pDev, PIRP pIrp){
	NTSTATUS status;
	PIO_STACK_LOCATION stack;
	unsigned long BufferLenght;
	void* buf;
	
	
	PDEVICE_EXTENSION DeviceExtension = pDev->DeviceExtension;
	DbgPrint("MYDRV: Start OnRead");

	pIrp->IoStatus.Information = 0;

	if(pDev->Flags & DO_BUFFERED_IO){
		buf = pIrp->AssociatedIrp.SystemBuffer;
	}else if(pDev->Flags & DO_DIRECT_IO){
		buf = MmGetSystemAddressForMdl(pIrp->MdlAddress);
	}else{
		buf = pIrp->UserBuffer;
	}

	stack = IoGetCurrentIrpStackLocation(pIrp);
	BufferLenght = stack->Parameters.Read.Length;

	RtlMoveMemory(
      buf,
      DeviceExtension->Image,
      BufferLenght>THE_BUFFER_LENGTH?THE_BUFFER_LENGTH:BufferLenght);
	  pIrp->IoStatus.Information = BufferLenght>THE_BUFFER_LENGTH?THE_BUFFER_LENGTH:BufferLenght;
	
	status=STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, 0);
	DbgPrint("MYDRV: Success OnRead");
	return status;
}


NTSTATUS OnWrite(PDEVICE_OBJECT pDev, PIRP pIrp){
	NTSTATUS status;
	PIO_STACK_LOCATION stack;
	unsigned long BufferLenght;
	void* buf;

	PDEVICE_EXTENSION DeviceExtension = pDev->DeviceExtension;
	DbgPrint("MYDRV: Start OnWrite");

	pIrp->IoStatus.Information = 0;
	
	if(pDev->Flags & DO_BUFFERED_IO){
		buf = pIrp->AssociatedIrp.SystemBuffer;
	}else if(pDev->Flags & DO_DIRECT_IO){
		buf = MmGetSystemAddressForMdl(pIrp->MdlAddress);
	}else{
		buf = pIrp->UserBuffer;
	}	
	stack = IoGetCurrentIrpStackLocation(pIrp);
	BufferLenght = stack->Parameters.Read.Length;

	RtlMoveMemory(
      DeviceExtension->Image,
      buf, 
      BufferLenght>THE_BUFFER_LENGTH?THE_BUFFER_LENGTH:BufferLenght);
	pIrp->IoStatus.Information = BufferLenght>THE_BUFFER_LENGTH?THE_BUFFER_LENGTH:BufferLenght;

	status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, 0);
	DbgPrint("MYDRV: Success OnWrite");
	return STATUS_SUCCESS;
}

NTSTATUS OnControl(PDEVICE_OBJECT pDev, PIRP pIrp){
	NTSTATUS status;
	PIO_STACK_LOCATION stack;
	unsigned long BufferLenght, method, InLength, OutLength;
	void* buf;
	void* InBuffer;
	void* OutBuffer;

	DbgPrint("MYDRV: Start OnControl");
	method = -1;
	stack = IoGetCurrentIrpStackLocation(pIrp);
	switch(stack->Parameters.DeviceIoControl.IoControlCode){
		case IOCTL_MY_BUFFERED:
			DbgPrint("MYDRV: IOCTL_MY_BUFFERED");
			InBuffer = pIrp->AssociatedIrp.SystemBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = pIrp->AssociatedIrp.SystemBuffer;
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		case IOCTL_MY_INDIRECT:
			DbgPrint("MYDRV: IOCTL_MY_INDIRECT");
			InBuffer = pIrp->AssociatedIrp.SystemBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = MmGetSystemAddressForMdl( pIrp->MdlAddress);
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		case IOCTL_MY_OUTDIRECT:
			DbgPrint("MYDRV: IOCTL_MY_OUTDIRECT");
			InBuffer = pIrp->AssociatedIrp.SystemBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = MmGetSystemAddressForMdl( pIrp->MdlAddress);
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		case IOCTL_MY_NEITHER:
			DbgPrint("MYDRV: IOCTL_MY_NEITHER");
			InBuffer = stack->Parameters.DeviceIoControl.Type3InputBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = pIrp->UserBuffer;
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		default:
			method = stack->Parameters.DeviceIoControl.IoControlCode & 0x03L;
			break;
	}
	
	if(method != -1){
		switch(method){
		case METHOD_BUFFERED:
			DbgPrint("MYDRV: METHOD_BUFFERED");
			InBuffer = pIrp->AssociatedIrp.SystemBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = pIrp->AssociatedIrp.SystemBuffer;
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		case METHOD_IN_DIRECT:
			DbgPrint("MYDRV: METHOD_IN_DIRECT");
			InBuffer = pIrp->AssociatedIrp.SystemBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = MmGetSystemAddressForMdl( pIrp->MdlAddress);
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		case METHOD_OUT_DIRECT:
			DbgPrint("MYDRV: METHOD_OUT_DIRECT");
			InBuffer = pIrp->AssociatedIrp.SystemBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = MmGetSystemAddressForMdl( pIrp->MdlAddress);
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		case METHOD_NEITHER:
			DbgPrint("MYDRV: METHOD_NEITHER");
			InBuffer = stack->Parameters.DeviceIoControl.Type3InputBuffer;
			InLength = stack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffer = pIrp->UserBuffer;
			OutLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
			break;
		}
	}


	DbgPrint("MYDRV: InBuf=%08lx InLen=%lx, OutBuf=%08lx OutLen=%lx\n",
		(ULONG)InBuffer, InLength,(ULONG)OutBuffer, OutLength);
	status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, 0);
	DbgPrint("MYDRV: Success OnControl");
	return status;
}