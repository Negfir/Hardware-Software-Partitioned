#include "systemc.h"
#include "interface.h"
#include "bus.h"
#include <fstream>
#define CLOCK_CYCLE 20.0/3

#define SIZE 6
#define LOOP 1000
#define a_ADDR 0
#define b_ADDR 36
#define c_ADDR 72
#define MEM_SIZE 108
//Operations
#define MEM_READ 0
#define MEM_Write 1
#define HW_OPs 2
#define CHECK_FLAG 3
//IDs
#define SW_id 0
#define HW_id 1


SC_MODULE (Software)
//class Software: public sc_module
{
    sc_port<bus_master_if> SW_port;


    unsigned int done_FLAG;

    sc_signal<int> counter;

    sc_in<sc_logic> clk;

    int cycle;
    
    
    unsigned result;
    


  void softwareFunction() 
  {
    int n;
    int i,j,k;

    unsigned int data=0;
    unsigned int init_i_loop=0;
    unsigned int init_j_loop=0;
    unsigned int mul_i_loop=0;
    unsigned int mul_j_loop=0;
    unsigned int sw_wait_loop=0;



    double start = sc_time_stamp().to_double ();

    

    for (n=0; n<LOOP; n++) 
    {
 
    cout<<endl<<"LOOP#"<<n;
    done_FLAG=0;

//################################ Software part ################################//
    int j_time1=0;
    double loop1_start = sc_time_stamp().to_double ();
    SW_port->Request(SW_id, c_ADDR, MEM_Write, 36);
    if(SW_port->WaitForAcknowledge(SW_id)){
    cout <<endl;
    //cout << "********************* Initializing Matrix *********************"<<endl;
      for(i=0; i<SIZE; i++) 
        {
        double loop2_start = sc_time_stamp().to_double ();
        for(j=0; j<SIZE; j++) 
            { 
          
            SW_port->WriteData(0);
            }
        double loop2_end = sc_time_stamp().to_double ();
        j_time1=j_time1+int((loop2_end-loop2_start)/CLOCK_CYCLE);
        init_j_loop=init_j_loop+int((loop2_end-loop2_start)/CLOCK_CYCLE);
        
        }
    }
        double loop1_end = sc_time_stamp().to_double ();
        //cout<<endl<<"Total cycles of loop j cycles: "<<j_time1<<endl;
        init_i_loop=init_i_loop+int((loop1_end-loop1_start)/CLOCK_CYCLE);
        


//################################ Hardware part ################################//
     int j_time2=0;
     double loop3_start = sc_time_stamp().to_double ();
      for(i=0; i<SIZE; i++) {
        double loop4_start = sc_time_stamp().to_double ();
        for(j=0; j<SIZE; j++) 
        {


            SW_port->Request(SW_id,SIZE, HW_OPs, 2);
            if(SW_port->WaitForAcknowledge(SW_id)){
                SW_port->WriteData(i);
                SW_port->WriteData(j);

            }

        }
        double loop4_end = sc_time_stamp().to_double ();
        j_time2=j_time2+int((loop4_end-loop4_start)/CLOCK_CYCLE);
        mul_j_loop=mul_j_loop+int((loop4_end-loop4_start)/CLOCK_CYCLE);
        
    }
        double loop3_end = sc_time_stamp().to_double ();
        //cout<<endl<<"Total cycles of loop j cycles: "<<j_time2<<endl;
        mul_i_loop=mul_i_loop+int((loop3_end-loop3_start)/CLOCK_CYCLE);
        

        //waiting for hardware
        double wait_start = sc_time_stamp().to_double ();
        while (done_FLAG!=1){
        wait(20*CLOCK_CYCLE);
        SW_port->Request(SW_id, 0, CHECK_FLAG, 1);
        if(SW_port->WaitForAcknowledge(SW_id)){ 
            SW_port->ReadData(done_FLAG);
        }   
        }
        double wait_end = sc_time_stamp().to_double ();
        sw_wait_loop = sw_wait_loop + int((wait_end-wait_start)/CLOCK_CYCLE);
        
    
    }

        long end = sc_time_stamp().to_double () ;
        cout.setf(ios::fixed, ios::floatfield);
        cout.setf(ios::showpoint);

        cout<<endl<<"Total HW/SW cycles: "<<int(((end-start)/CLOCK_CYCLE)+1)/1000<<endl;

        cout<<endl<<"Initialization loop i cycles: "<<init_i_loop/1000<<endl;
        cout<<"Initialization loop j cycles: "<<init_j_loop/6000<<endl;
        
        cout<<endl<<"Multiplying loop i cycles: "<<mul_i_loop/1000<<endl;
        cout<<"Multiplying loop j cycles: "<<mul_j_loop/6000<<endl;
        cout<<"Waiting for hardware cycles: "<<sw_wait_loop/1000<<endl;

        sc_stop();

    return ;

  }









//   void softwareOnly() 
//   {
//     int n;
//     int i,j,k;

//     unsigned int data=0;
//     unsigned int init_i_loop=0;
//     unsigned int init_j_loop=0;
//     unsigned int mul_i_loop=0;
//     unsigned int mul_j_loop=0;
//     unsigned int mul_k_loop=0;
//     unsigned int sw_wait_loop=0;



//     double start = sc_time_stamp().to_double ();

    

//     for (n=0; n<LOOP; n++) 
//     {
 
//     //cout<<endl<<"LOOP#"<<n;
//     done_FLAG=0;

// //################################ Software part ################################//
//     int j_time1=0;
//     double loop1_start = sc_time_stamp().to_double ();
//     SW_port->Request(SW_id, c_ADDR, MEM_Write, 36);
//     if(SW_port->WaitForAcknowledge(SW_id)){
//     cout <<endl;
//     //cout << "********************* Initializing Matrix *********************"<<endl;
//       for(i=0; i<SIZE; i++) 
//         {
//         double loop2_start = sc_time_stamp().to_double ();
//         for(j=0; j<SIZE; j++) 
//             { 
          
//             SW_port->WriteData(0);
//             }
//         double loop2_end = sc_time_stamp().to_double ();
//         j_time1=j_time1+int((loop2_end-loop2_start)/CLOCK_CYCLE);
//         init_j_loop=init_j_loop+int((loop2_end-loop2_start)/CLOCK_CYCLE);
        
//         }
//     }
//         double loop1_end = sc_time_stamp().to_double ();
//         //cout<<endl<<"Total cycles of loop j cycles: "<<j_time1<<endl;
//         init_i_loop=init_i_loop+int((loop1_end-loop1_start)/CLOCK_CYCLE);
        


// //################################ Hardware part ################################//
//     unsigned int a=0;
//     unsigned int b=0;
//     unsigned int c=0;

//      double loop3_start = sc_time_stamp().to_double ();
//       for(i=0; i<SIZE; i++) {
//         double loop4_start = sc_time_stamp().to_double ();
//         for(j=0; j<SIZE; j++) 
//         {
//             double loop5_start = sc_time_stamp().to_double ();
//             for(k=0; k<SIZE; k++) 
//             {

//             SW_port->Request(SW_id,a_ADDR+i*SIZE+k, MEM_READ, 1);
//             if(SW_port->WaitForAcknowledge(SW_id)){
//                 SW_port->ReadData(a);
//             }

//             SW_port->Request(SW_id,b_ADDR+k*SIZE+j, MEM_READ, 1);
//             if(SW_port->WaitForAcknowledge(SW_id)){
//                 SW_port->ReadData(b);
//             }
//             c=c+ a*b;
//             wait(2*CLOCK_CYCLE);
//         }

//             SW_port->Request(SW_id,c_ADDR+i*SIZE+j, MEM_Write, 1);
//             if(SW_port->WaitForAcknowledge(SW_id)){
//                 SW_port->WriteData(c);
//             }
//             c=0;
//         double loop5_end = sc_time_stamp().to_double ();
//         mul_k_loop=mul_k_loop+int((loop5_end-loop5_start)/CLOCK_CYCLE);            

//         }
//         double loop4_end = sc_time_stamp().to_double ();
//         mul_j_loop=mul_j_loop+int((loop4_end-loop4_start)/CLOCK_CYCLE);
        
//     }
//         double loop3_end = sc_time_stamp().to_double ();
//         //cout<<endl<<"Total cycles of loop j cycles: "<<j_time2<<endl;
//         mul_i_loop=mul_i_loop+int((loop3_end-loop3_start)/CLOCK_CYCLE);
        


   

    
//     }

//         long end = sc_time_stamp().to_double () ;
//         cout.setf(ios::fixed, ios::floatfield);
//         cout.setf(ios::showpoint);

//         cout<<endl<<"Total HW/SW cycles: "<<int(((end-start)/CLOCK_CYCLE)+1)/1000<<endl;

//         cout<<endl<<"Initialization loop i cycles: "<<init_i_loop/1000<<endl;
//         cout<<"Initialization loop j cycles: "<<init_j_loop/6000<<endl;
        
//         cout<<endl<<"Multiplying loop i cycles: "<<mul_i_loop/1000<<endl;
//         cout<<"Multiplying loop j cycles: "<<mul_j_loop/6000<<endl;
//         cout<<"Multiplying loop k cycles: "<<mul_k_loop/36000<<endl;


//         sc_stop();

//     return ;

//   }







    


  SC_CTOR(Software) {
    result =0;
    cycle=0;

    SC_THREAD(softwareFunction);
    // SC_THREAD(softwareOnly);

  
    sensitive << clk.pos();
    
}

  
};
