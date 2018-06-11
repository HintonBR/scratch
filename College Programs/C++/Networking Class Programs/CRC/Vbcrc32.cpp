// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: vbcrc32.cpp 
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: Doug Gaer 
// File Creation Date: 08/17/1998 
// Date Last Modified: 02/13/2001
// ----------------------------------------------------------- // 
// ------------- Program description and details ------------- // 
// ----------------------------------------------------------- // 
/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
 
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
USA
  
The CRC32 functions (Cyclic Redundancy Check) are used to
calculate a sophisticated checksum based on the algebra of
polynomials. The Cyclic Redundancy Check, is a way to detect
bit errors that occur during data storage or transmission.
The CRC-32 algorithm operates on a block of data as a single
large numerical value. The algorithm divides this large value
by the CRC-32 polynomial or generator polynomial, leaving the
remainder 32-bit, which is the checksum. 
*/
// ----------------------------------------------------------- // 
#ifdef __USE_CRC32_TABLE_FUNCTIONS__
#include <iomanip.h>
#endif

#include "vbcrc32.h"
#include "vbcrctab.h"

unsigned long calcCRC32(const char *buf, unsigned len)
// Calculate a 32-bit CRC for a raw pattern of bytes.
// Returns a 32-bit checksum.
{
 unsigned long CRC=0xffffffffL;
 unsigned int n = len;

 while(n--) 
   CRC=crc32tab[(CRC ^ (*buf++)) & 0xFF] ^ ((CRC>>8) & 0x00ffffffL);

 return CRC ^ 0xffffffffL;
}

unsigned long calcCRC32(unsigned char *buf, unsigned len)
// Calculate a 32-bit CRC for a raw pattern of bytes.
// Returns a 32-bit checksum.
{
 unsigned long CRC=0xffffffffL;
 unsigned int n = len;
 unsigned char *p = buf;
 while(n--) 
   CRC=crc32tab[(CRC ^ (*p++)) & 0xFF] ^ ((CRC>>8) & 0x00ffffffL);

 return CRC ^ 0xffffffffL;
}

unsigned long calcCRC32(unsigned char c, unsigned long CRC)
// Calculate a 32-bit CRC table value for a single byte.
// NOTE: The initial CRC value must be set to 0xffffffffL
// and the final 32-bit value that must be XOR'ed with
// 0xffffffffL to obtain the checksum.
{
  unsigned int i;
  i = (unsigned int)c;
  i &= 0xFF; // Reset all the bits
  CRC=crc32tab[(CRC ^ i) & 0xFF] ^ ((CRC>>8) & 0x00ffffffL);
  return CRC;
}

#ifdef __USE_CRC32_TABLE_FUNCTIONS__
unsigned long calcCRC32(fstream &infile)
// Calculate a 32-bit CRC for a file. Assumes the stream
// is already open. Returns a 32-bit checksum.
{
 unsigned long CRC=0xffffffffL;
 unsigned char c;
 unsigned int i;

 // Rewind to the start of the stream
 infile.clear(); 
 infile.seekg(0, ios::beg);
 infile.seekp(0, ios::beg);

 while(!infile.eof()) {
   infile.get(c);
   i = (unsigned int)c;
   i &= 0xFF; // Reset all the bits
   if(infile.eof()) break;
   CRC=crc32tab[(CRC ^ i) & 0xFF] ^ ((CRC>>8) & 0x00ffffffL);
 }
 return CRC ^ 0xffffffffL;
}

int makeCRC32(ostream &stream)
// Write a CRC 32 table for a byte-wise 32-bit CRC calculation
// based on the Autodin/Ethernet/ADCCP polynomial of 0x4C11DB7:
// 0000 0100 1100 0001 0001 1101 1011 0111 (binary) or a poly of
// x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7 +x^5+x^4+x^2+x^1+x^0
// In this representation the coefficient of x^0 is stored in the 
// MSB of the 32-bit word and the coefficient of x^31 is stored in 
// the LSB. Thus 0x4C11DB7 becomes 0xEDB88320:
// 1110 1101 1011 1000 1000 0011 0010 0000 (binary)

// Adding the polynomials is performed using an exclusive-or, and
// multiplying a polynomial by x is a right shift by one. If the 
// polynomial is called "p", and each byte is represented as polynomial
// "q", with the lowest power in the most significant bit (so the byte
// 0xb1 is the polynomial x^7+x^3+x+1), then the CRC is (q*x^32) 
// mod "p", where "a" mod "b" means the remainder after dividing "a" by
// "b". This calculation is done using the shift-register method of
// multiplying and taking the remainder. The register is initialized
// to zero, and for each incoming bit, x^32 is added mod "p" to the
// register if the bit is a one (where x^32 mod p is p+x^32 = x^26+...+1),
// and the register is multiplied mod "p" by "x" (which is shifting right
// by one and adding x^32 mod "p" if the bit shifted out is a one.) This
// algorithm starts with the highest power (least significant bit) of "q"
// and repeats for all eight bits of "q".
{
  unsigned long c; // CRC shift register 
  unsigned long e; // Polynomial exclusive-or pattern 
  int i;           // Counter for all possible eight bit values 
  int k;           // Byte being shifted into crc apparatus 

  // Terms of polynomial defining this crc (except x^32): 
  static const int p[14] = {0,1,2,4,5,7,8,10,11,12,16,22,23,26};

  // Make exclusive-or pattern from polynomial 
  e = 0;
  for (i = 0; i < sizeof(p)/sizeof(int); i++)
    e |= 1L << (31 - p[i]);

  // Compute and print table of CRC's, five per line 
  stream << endl;
  stream << "  0x00000000U";
  for (i = 1; i < 256; i++)
  {
    c = 0;
    for (k = i | 256; k != 1; k >>= 1)
    {
      c = c & 1 ? (c >> 1) ^ e : c >> 1;
      if (k & 1)
        c ^= e;
    }

    if(i % 5)
      stream << ", 0x" << setfill('0') << setw(8) << hex << c << "U";
    else
      stream << "," << endl << "  0x" << setfill('0') << setw(8) << hex
	     << c << "U";
  }
  stream << endl;
  return 1;
}
#endif // __USE_CRC32_TABLE_FUNCTIONS__
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
