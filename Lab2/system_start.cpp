#include <iostream>
#include "systemc.h"
#include "model/core.h"
#include "model/core_last.h"
#include "model/memory.h"
#include "model/bus.h"
#include "model/IO.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int sc_main(int argc, char* argv[]) {

	IO IO("IOput");
	bus bus("bus");
	core core1("core1"),core2("core2"),core3("core3");
	core_last core_last("core_last");
	memory memory("memory");
	
	
	sc_clock clk("clk", sc_time(10, SC_NS));
	sc_signal<bool> wr_i_IO;
	sc_signal<float> data_i_IO[49];
	sc_signal<bool> wr_o_IO;
	sc_signal<float> data_o_IO[3];

	sc_signal<bool> wr_i_core1;
	sc_signal<bool> rd_i_core1;
	sc_signal<int> addr_i_core1;
	sc_signal<bool> wr_o_core1;
	sc_signal<float> data_ow_core1[49];
	sc_signal<float> data_o_core1[49];

	sc_signal<bool> wr_i_core2;
	sc_signal<bool> rd_i_core2;
	sc_signal<int> addr_i_core2;
	sc_signal<bool> wr_o_core2;
	sc_signal<float> data_ow_core2[49];
	sc_signal<float> data_o_core2[49];

	sc_signal<bool> wr_i_core3;
	sc_signal<bool> rd_i_core3;
	sc_signal<bool> wr_o_core3;
	sc_signal<int> addr_i_core3;
	sc_signal<float> data_ow_core3[49];
	sc_signal<float> data_o_core3[49];

	sc_signal<bool> wr_i_corelast;
	sc_signal<bool> rd_i_corelast;
	sc_signal<int> addr_i_corelast;
	sc_signal<float> data_i_corelast[3];
	sc_signal<float> data_ow_corelast[30];

	sc_signal<bool> wr_o_mem;
	sc_signal<bool> rd_o_mem;
	sc_signal<int> addr_o_mem;
	sc_signal<float> data_i_mem[49];
	sc_signal<float> data_o_mem[3];

	sc_signal <bool> wr_co_core[3];
	sc_signal <float> data_co_core[30];
	
	//INPUT

	IO.clk_i(clk);
	for_train()
	{
		IO.data_o[i](data_i_IO[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		IO.data_i[i](data_o_IO[i]);
	}
	IO.wr_i(wr_o_IO);
	IO.wr_o(wr_i_IO);
	
	//BUS
	bus.clk_i(clk);
	bus.wr_i_IO(wr_i_IO);
	for (int i = 0; i < 49; i++)
	{
		bus.data_i_IO[i](data_i_IO[i]);
	}
	bus.wr_o_IO(wr_o_IO);
	for (int i = 0; i < 3; i++)
	{
		bus.data_o_IO[i](data_o_IO[i]);
	}	

	bus.wr_i_core1(wr_i_core1);
	bus.rd_i_core1(rd_i_core1);
	bus.addr_i_core1(addr_i_core1);
	bus.wr_o_core1(wr_o_core1);
	for (int i = 0; i < 49; i++)
	{
		bus.data_ow_core1[i](data_ow_core1[i]);
		bus.data_o_core1[i](data_o_core1[i]);
	}

	bus.wr_i_core2(wr_i_core2);
	bus.rd_i_core2(rd_i_core2);
	bus.addr_i_core2(addr_i_core2);
	bus.wr_o_core2(wr_o_core2);
	for (int i = 0; i < 49; i++)
	{
		bus.data_ow_core2[i](data_ow_core2[i]);
		bus.data_o_core2[i](data_o_core2[i]);
	}

	bus.wr_i_core3(wr_i_core3);
	bus.rd_i_core3(rd_i_core3);
	bus.addr_i_core3(addr_i_core3);
	bus.wr_o_core3(wr_o_core3);
	for (int i = 0; i < 49; i++)
	{
		bus.data_ow_core3[i](data_ow_core3[i]);
		bus.data_o_core3[i](data_o_core3[i]);
	}
	bus.wr_i_corelast(wr_i_corelast);
	bus.rd_i_corelast(rd_i_corelast);
	bus.addr_i_corelast(addr_i_corelast);
	//bus.wr_o_corelast(wr_o_core_last);
	for (int i = 0; i < 3; i++)
	{
		bus.data_i_corelast[i](data_i_corelast[i]);
	}
	for (int i = 0; i < 30; i++)
	{
		bus.data_ow_corelast[i](data_ow_corelast[i]);
	}

	bus.wr_o_mem(wr_o_mem);
	bus.rd_o_mem(rd_o_mem);
	bus.addr_o_mem(addr_o_mem);
	for (int i = 0; i < 49; i++)
	{
		bus.data_i_mem[i](data_i_mem[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		bus.data_o_mem[i](data_o_mem[i]);
	}

	// CORE 1

	core1.clk_i(clk);
	core1.wr_ci(wr_o_core1);
	for_train()
	{
		core1.data_bi[i](data_ow_core1[i]);
		core1.data_ci[i](data_o_core1[i]);
	}
	core1.wr_co(wr_co_core[0]);
	for (int i(0); i < 10; i++)
	{
		core1.data_co[i](data_co_core[i]);
	}
	core1.rd_bo(rd_i_core1);
	core1.wr_bo(wr_i_core1);
	core1.addr_bo(addr_i_core1);
	
	//CORE 2

	core2.clk_i(clk);
	core2.wr_ci(wr_o_core2);
	for_train()
	{
		core2.data_bi[i](data_ow_core2[i]);
		core2.data_ci[i](data_o_core2[i]);
	}
	core2.wr_co(wr_co_core[1]);
	for (int i(0); i < 10; i++)
	{
		core2.data_co[i](data_co_core[i+10]);
	}
	core2.rd_bo(rd_i_core2);
	core2.wr_bo(wr_i_core2);
	core2.addr_bo(addr_i_core2);

	//CORE 3

	core3.clk_i(clk);
	core3.wr_ci(wr_o_core3);
	for_train()
	{
		core3.data_bi[i](data_ow_core3[i]);
		core3.data_ci[i](data_o_core3[i]);
	}
	core3.wr_co(wr_co_core[2]);
	for (int i(0); i < 10; i++)
	{
		core3.data_co[i](data_co_core[i+20]);
	}
	core3.rd_bo(rd_i_core3);
	core3.wr_bo(wr_i_core3);
	core3.addr_bo(addr_i_core3);

	core_last.clk_i(clk);
	for(int i=0;i<30;i++)
	{
		core_last.data_bi[i](data_ow_corelast[i]);
		core_last.data_ci[i](data_co_core[i]);
	}
	for (int i(0); i < 3; i++)
	{
		core_last.wr_ci[i](wr_co_core[i]);
		core_last.data_bo[i](data_i_corelast[i]);
	}
	core_last.rd_bo(rd_i_corelast);
	core_last.wr_bo(wr_i_corelast);
	core_last.addr_bo(addr_i_corelast);

   	
	// MEMORY
	
	memory.clk_i(clk);
	memory.addr_bi(addr_o_mem);	
	for (int i = 0; i < 3; i++)
	{
		memory.data_bi[i](data_o_mem[i]);
	}
	memory.rd_i(rd_o_mem);	
	memory.wr_i(wr_o_mem);
	for (int i = 0; i < 49; i++)
	{
		memory.data_bo[i](data_i_mem[i]);
	}
	

	// TRACING

	//sc_trace_file *wf = sc_create_vcd_trace_file("wave");

	cout << endl << "_________________________" << endl << endl << "Calculating..." << endl << "_________________________" << endl;
	sc_start(sc_time(4000, SC_NS));

	//sc_close_vcd_trace_file(wf);
	
	return(0);
}