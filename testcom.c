/*************************************************************
  TESTCOM
  A program to test rs232.c routines for com ports on Windows/Linux
usage:
  testcom port baud_rate parity data_bits stop_stop
   eg   testcom 5 9600 n 8 1
        for com5 at 4800 baud with no parity, 8 data and 1 stop


 uses the rs232.c routines from ??????
 compile gcc -o testcom testcom.c rs232.c
*************************************************************/
#include <stdio.h>
#include <stdlib.h>


#include "rs232.h"

main(int argc, char **argv)
{
/* loop  counters */
    int i,j=0;
/* program variables */
    int port_number,baud_rate,data_bits,stop_bits,n;
    char parity,OutputChar,SendBuffer[80]= "testcom: Test Send Buffer";
/* serial port buffer size */
    int size=4096;
    char buf[size];

/* read and check the command line variable */
    if (argc != 6)
    {
      fprintf(stderr,"%s: Requires 5 command line variables\n",argv[0]);
      fprintf(stderr,"%s: portnum baud parity data stop\n",argv[0]);
      fprintf(stderr,"%s: eg. %s 3 4800 n 8 1\n",argv[0],argv[0]);
      exit(-1);
    }
    if ( sscanf(argv[1], "%d", &port_number) != 1)
    {
      fprintf(stderr,"%s: Error reading port from command line\n",argv[0]);
      exit(-1);
    }
   if (port_number > 16)
   {
       fprintf(stderr,"%s: Serial Ports Max out at 16\n",argv[0]);
       exit(-1);
   }
   printf("port_number = %d\n",port_number);
   port_number = port_number-1;
   if ( sscanf(argv[2], "%d", &baud_rate) != 1)
   {
       fprintf(stderr,"%s: Error reading baudrate  from command line\n",argv[0]);
       exit(-1);
   }
   printf("baud_rate = %d\n",baud_rate);
   if ( sscanf(argv[3], "%c", &parity) != 1)
   {
       fprintf(stderr,"%s: Error reading parity  from command line\n",argv[0]);
       exit(-1);
   }
   printf("parity = %c\n",parity);
   if ( sscanf(argv[4], "%d", &data_bits) != 1)
   {
       fprintf(stderr,"%s: Error reading data_bits from command line\n",argv[0]);
       exit(-1);
   }
   printf("data_bits = %d\n",data_bits);
   if ( sscanf(argv[5], "%d", &stop_bits) != 1)
   {
       fprintf(stderr,"%s: Error reading stop_bits from command line\n",argv[0]);
       exit(-1);
   }
   printf("stop_bits = %d\n",stop_bits);
/* Finished checking the command line variables */

/* Now start working with the com port routines */
   if (OpenComport(port_number,baud_rate)==1)
   {
      fprintf(stderr,"%s:Error opening serial port\n",argv[0]);
      exit(-1);
   }
/* is CTS enabled? */
  if (IsCTSEnabled(port_number) == 1)
  {
    printf("%s: ** CTS is enabled **\n",argv[0]);
  }

  printf("%s: Waiting to receive data. Auto time-out\n",argv[0]);
  j=0;
  while ( j< 10000000)
//  while (1)
   {
    n=PollComport(port_number,buf,size);
    if (n != 0)
    {
     for (i=0; i<n; i++)
         printf("%c",buf[i]);
         fflush(stdout);
    }
    ++j;
   }
  /* send bytes out from the comport */
   printf("%s: Test sending data  CTRL-C to End\n",argv[0]);
   j=0;
   SendByte(port_number,'\n');
   SendBuf(port_number,SendBuffer,80);
   while (1)
   {
    OutputChar=getc(stdin);
    if (SendByte(port_number,OutputChar) == 1)
    {
       fprintf(stderr,"%s: Error sending data\n",argv[0]);
       exit(-1);
    }
    ++j;
   }

   CloseComport(port_number);

} /* end of main */

