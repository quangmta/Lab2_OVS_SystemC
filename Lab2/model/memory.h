#pragma once

#include "systemc.h"
#include "IO.h"

using namespace std;


SC_MODULE(memory) {

	sc_in<bool>  clk_i;
	sc_in<int>   addr_bi;		// reading address
	sc_in<float>   data_bi[3];		// reading data
	sc_in<bool>  rd_i;		// reading flag
	sc_in<bool>  wr_i;					// writing flag
	sc_out<float>  data_bo[49];				// writing data

	SC_HAS_PROCESS(memory);


	// bus main thread
	void mem_read()
	{
		if (rd_i.read())
			for (int i(0); i < 49; i++)
			{
				data_bo[i].write(mem[addr_bi.read()][i]);
			}			
	}

	void mem_write()
	{
		if (wr_i.read())
			for (int i(0); i < 3; i++)
			{
				mem[addr_bi.read()][i] = data_bi[i].read();
			}
			
	}

	void read_data()
	{
		ifstream fin("data/weight.txt");
		while (!fin.eof()) {
			for (int i(0); i < 30; i++)
				for (int j(0); j < 49; j++)
				{
					fin >> mem[i][j];
				}
			for (int i(0); i < 3; i++)
				for (int j(0); j < 30; j++)
				{
					fin >> mem[i+30][j];
				}
		}
	}

	SC_CTOR(memory) 
	{
		for (int i(0); i < 49; i++)
		{
			data_bo[i].initialize(0);
		}
		mem.resize(50);
		for (int i(0); i < 50; i++)
			mem[i].resize(49);

		SC_METHOD(mem_write);
		sensitive << clk_i.pos();

		SC_METHOD(mem_read);
		sensitive << clk_i.pos();

		SC_CTHREAD(read_data, clk_i.pos());
	}


private:
	vector<vector<float>> mem;
};