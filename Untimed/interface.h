#include "systemc.h"

#ifndef __interface_H_INCLUDED__   
#define __interface_H_INCLUDED__

// Bus Master Interface
// class bus_master_if : virtual public sc_interface
// {
//   public:
//     virtual void Request(unsigned int mst_id, unsigned int addr, unsigned int op, unsigned int len) = 0;
//     virtual bool WaitForAcknowledge(unsigned int mst_id) = 0;
//     virtual void ReadData(unsigned int &data) = 0;
//     virtual void WriteData(unsigned int data) = 0;
// };


// // Bus Minion Interface 
// class bus_minion_if : virtual public sc_interface
// {
//   public:
//     virtual void Listen(unsigned int &req_addr, unsigned int &req_op, unsigned int &req_len) = 0;
//     virtual void Acknowledge() = 0; 
//     virtual void SendReadData(unsigned int data) = 0;
//     virtual void ReceiveWriteData(unsigned int &data) = 0;
// };

class simple_mem_if: virtual public sc_interface
{
public:
  virtual bool Write(unsigned int addr, unsigned int data) = 0; 
  virtual bool Read(unsigned int addr, unsigned int& data) = 0; 
};

#endif