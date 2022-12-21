#pragma once

#include "systemc.h"
#include "IO.h"

using namespace std;


SC_MODULE(bus) {

	sc_in<bool>  clk_i;

	sc_in<bool> wr_i_IO;
	sc_in<float> data_i_IO[49];
	sc_out<bool> wr_o_IO;
	sc_out<float> data_o_IO[3];

	sc_in<bool> wr_i_core1;
	sc_in<bool> rd_i_core1;
	sc_in<int> addr_i_core1;
	sc_out<bool> wr_o_core1;
	sc_out<float> data_ow_core1[49];
	sc_out<float> data_o_core1[49];

	sc_in<bool> wr_i_core2;
	sc_in<bool> rd_i_core2;
	sc_in<int> addr_i_core2;
	sc_out<bool> wr_o_core2;
	sc_out<float> data_ow_core2[49];
	sc_out<float> data_o_core2[49];

	sc_in<bool> wr_i_core3;
	sc_in<bool> rd_i_core3;
	sc_out<bool> wr_o_core3;
	sc_in<int> addr_i_core3;
	sc_out<float> data_ow_core3[49];
	sc_out<float> data_o_core3[49];

	sc_in<bool> wr_i_corelast;
	sc_in<bool> rd_i_corelast;
	sc_in<int> addr_i_corelast;
	sc_in<float> data_i_corelast[3];
	sc_out<float> data_ow_corelast[30];

	sc_out<bool> wr_o_mem;
	sc_out<bool> rd_o_mem;
	sc_out<int> addr_o_mem;
	sc_in<float> data_i_mem[49];
	sc_out<float> data_o_mem[3];


	// bus main thread
	void bus_process()
	{
		for (int i = 0; i < 10; i++)
		{
			while (!rd_i_core1.read()) wait();
			addr_o_mem.write(addr_i_core1.read());
			rd_o_mem.write(1);
			wait();
			rd_o_mem.write(0);
			wait();
			for (int i = 0; i < 49; i++)
			{
				float a = data_i_mem[i].read();
				data_ow_core1[i].write(a);
			}
		}

		
		for (int i = 0; i < 10; i++)
		{
			while (!rd_i_core2.read()) wait();
			addr_o_mem.write(addr_i_core2.read());
			rd_o_mem.write(1);
			wait();
			rd_o_mem.write(0);
			wait();
			for (int i = 0; i < 49; i++)
			{
				data_ow_core2[i].write(data_i_mem[i].read());
			}
		}

		for (int i = 0; i < 10; i++)
		{
			while (!rd_i_core3.read()) wait();
			addr_o_mem.write(addr_i_core3.read());
			rd_o_mem.write(1);
			wait();
			rd_o_mem.write(0);
			wait();
			for (int i = 0; i < 49; i++)
			{
				data_ow_core3[i].write(data_i_mem[i].read());
			}
		}

		for (int i = 0; i < 3; i++)
		{
			while (!rd_i_corelast.read()) wait();
			addr_o_mem.write(addr_i_corelast.read());
			rd_o_mem.write(1);
			wait();
			rd_o_mem.write(0);
			wait();
			for (int i = 0; i < 30; i++)
			{
				data_ow_corelast[i].write(data_i_mem[i].read());
			}
		}

	}
	void bus_I()
	{
		if (wr_i_IO.read())
		{
			for (int i = 0; i < 49; i++)
			{
				wr_o_core1.write(1);
				wr_o_core2.write(1);
				wr_o_core3.write(1);
				data_o_core1[i].write(data_i_IO[i].read());
				data_o_core2[i].write(data_i_IO[i].read());
				data_o_core3[i].write(data_i_IO[i].read());
			}
		}
		else
		{
			wr_o_core1.write(0);
			wr_o_core2.write(0);
			wr_o_core3.write(0);
		}
	}
	void bus_O()
	{
		if (wr_i_corelast.read())
		{
			for (int i = 0; i < 3; i++)
			{
				//wr_o_mem.write(1);
				//addr_o_mem.write(addr_i_corelast.read());
				//data_o_mem[i].write(data_i_corelast[i].read());
				wr_o_IO.write(1);
				data_o_IO[i].write(data_i_corelast[i].read());
			}
		}
		else
		{
			wr_o_IO.write(0);
		}
	}



	SC_CTOR(bus) {

		wr_o_IO.initialize(0);
		for (int i = 0; i < 3; i++)
		{
			data_o_IO[i].initialize(0);
			data_o_mem[i].initialize(0);
		}

		for (int i = 0; i < 49; i++)
		{
			data_o_core1[i].initialize(0);
			data_o_core2[i].initialize(0);
			data_o_core3[i].initialize(0);
		}
		for (int i = 0; i < 30; i++)
		{
			data_ow_corelast[i].initialize(0);
		}

		addr_o_mem.initialize(0);

		SC_CTHREAD(bus_process, clk_i.pos());

		SC_METHOD(bus_I, clk_i.pos());
		sensitive << clk_i.pos();

		SC_METHOD(bus_O, clk_i.pos());
		sensitive << clk_i.pos();

		//SC_METHOD(bus_core1, clk_i.pos());
		//sensitive << clk_i.pos();

		//SC_METHOD(bus_core2, clk_i.pos());
		//sensitive << clk_i.pos();

		//SC_METHOD(bus_core3, clk_i.pos());
		//sensitive << clk_i.pos();

		//SC_METHOD(bus_corelast, clk_i.pos());
		//sensitive << clk_i.pos();

	}

	//private:
	//	int mem[9];

};