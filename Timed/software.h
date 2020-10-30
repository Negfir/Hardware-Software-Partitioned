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

    unsigned int a[SIZE][SIZE];
    unsigned int b[SIZE][SIZE];
    unsigned int c[SIZE][SIZE];
    unsigned int done_FLAG;

    sc_in<sc_logic> clk;

    int cycle;
    
    
    unsigned result;
    


  void softwareFunction() 
  {
    int n;
    int i,j,k;

    unsigned int data=0;
    double start = sc_time_stamp().to_double ();

    

    for (n=0; n<LOOP; n++) 
    {
    //sc_time loop_start = sc_core:: sc_time_stamp();
    double loop_start = sc_time_stamp().to_double ();
    int cycle1=cycle;
    //cout << "@"<<sc_time_stamp() << " LOOP #"<< n <<endl;
    done_FLAG=0;
    SW_port->Request(SW_id, c_ADDR, MEM_Write, 36);
    if(SW_port->WaitForAcknowledge(SW_id)){
    cout <<endl;
    cout << "********************* Initializing Matrix *********************"<<endl;
      for(i=0; i<SIZE; i++) 
        {
        for(j=0; j<SIZE; j++) 
            {
        
          
            SW_port->WriteData(0);
            }
        }
    }
      for(i=0; i<SIZE; i++) 
        for(j=0; j<SIZE; j++) 
        {
            SW_port->Request(SW_id,SIZE, HW_OPs, 2);
            if(SW_port->WaitForAcknowledge(SW_id)){
                SW_port->WriteData(i);
                SW_port->WriteData(j);

            }


        }

        while (done_FLAG!=1){
        SW_port->Request(SW_id, 0, CHECK_FLAG, 1);
        if(SW_port->WaitForAcknowledge(SW_id)){ 
            SW_port->ReadData(done_FLAG);
        }   
        }

        //sc_time loop_end = sc_core:: sc_time_stamp();
        double loop_end = sc_time_stamp().to_double ();
        double loop_cycles=(loop_end-loop_start)/CLOCK_CYCLE;
        cout<<endl<<"LOOP #"<<n<<" cycles: "<<loop_cycles<<endl<<endl;
        int cycle2=cycle;
        cout<<endl<<"LOOP #"<<n<<" cycles: "<<cycle2-cycle1<<endl<<endl;
    }
        //cout<<"||||||||||||"<<sc_time_stamp().to_seconds()*1000000000/CLOCK_CYCLE ;
        long end = sc_time_stamp().to_double () ;
        cout.setf(ios::fixed, ios::floatfield);
        cout.setf(ios::showpoint);

        cout<<endl<<"Total HW/SW cycles: "<<(end-start)/CLOCK_CYCLE<<endl<<endl;

        sc_stop();

    return ;

  }
    


  SC_CTOR(Software) {
    result =0;
    cycle=0;

    SC_THREAD(softwareFunction);
  
    sensitive << clk.pos();
    
}

  
};
