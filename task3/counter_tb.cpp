#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

#include "iostream"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init ton veriloa instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open("counter.vcd");

    // init buddy 
    //if (vbdOpen() !=1) return(-1) ;
    while (vbdOpen() != 1){
        printf("connecting");
    }
    vbdClear();
    vbdHeader("Lab 1: Counter");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;
    top->v = 50;
    vbdSetMode(1);

    int stop_cycles = 3;

    // run simulation for many clock cycles
    for (int i = 0; i < 1024; i++){

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);    // unit is in ps!!!
            top->clk = !top->clk;
            top->eval();
        }

        //Send count value to buddy 
        vbdPlot(int(top->count), 0, 255);
        /*
        vbdHex (4, (int (top->count) >> 16) & 0xF) ; 
        vbdHex (3, (int (top->count) >> 8) & 0xF) ; 
        vbdHex(2, (int (top->count) >> 4) & 0xF) ; 
        vbdHex(1, int (top->count) & 0xF) ;
        vbdCycle (i+1);
        */
        //end of buddy output sectiPon
        
        top->ld = vbdFlag();
        std::cout << vbdFlag() << std::endl;
        std::cout << int(top->count) << std::endl;
        top->rst = 0;

        if (Verilated::gotFinish()) exit(0);

    }
    vbdClose();
    tfp ->close();
    exit(0);

}