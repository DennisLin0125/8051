#define uchar unsigned char

#define DeviceAddressWR 0xae   // 1010 1110
#define DeviceAddressRD 0xaf   // 1010 1111

void delay();
void START();
void STOP();
void response();
void init();
void write_byte(uchar mydata);
uchar read_byte();
void delay1(uchar x);
void WriteDataToAddress(uchar WRaddress,uchar WRdata);
uchar RandomReadFromAddress(uchar RDaddress);
