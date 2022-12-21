#pragma once
#include "systemc.h"
#include "main.h"
#include "IO.h"

SC_MODULE(core) {	

	sc_in<bool>  clk_i;
	sc_in <bool> wr_ci;
	sc_in <float> data_ci[49];
	sc_out <bool> wr_co;
	sc_out <float> data_co[10];
	sc_out<bool> rd_bo;					
	sc_out<bool> wr_bo;						
	sc_out<int>  addr_bo;				
	sc_in <float>  data_bi[49];	

	static int count;
	int core_num;


	// core main method
	void input_read()
	{
		if (wr_ci.read())
		{
			//cout <<"core: "<< core_num<<" input: "<<sc_time_stamp() << endl;
			for_train()
			{
				test[i] = (float)data_ci[i].read();
			}			
			for (int i(0); i < 10; i++)
			{
				out_hidden[i] = 0;
				for (int j(0); j < 49; j++)
				{
					out_hidden[i] += test[j] * weight[i][j];
				}
				out_hidden[i] = 1 / (1 + exp(-out_hidden[i]));
			}
			wr_co.write(1);
			for (int i(0); i < 10; i++)
			{
				data_co[i].write(out_hidden[i]);
			}
		}
		else
		{
			wr_co.write(0);
		}

	}
	//core main thread
	void weight_read() {
		for (int i = 0; i < (core_num - 1) * 40; i++) wait();
		for (int i(0); i < 10; i++)
		{
			addr_bo.write(i+(core_num-1)*10);
			rd_bo.write(1);
			wait();
			wait();
			rd_bo.write(0);
			wait();
			wait();

			for (int j = 0; j < 49; j++)
			{
				weight[i][j] = data_bi[j].read();
			}
		}
		//cout << weight[0][0]<<endl;
		//cout << sc_time_stamp() << endl;
	}

	SC_CTOR(core) {

		count++;
		core_num = count;

		addr_bo.initialize(0);
		wr_co.initialize(0);
		rd_bo.initialize(0);
		wr_bo.initialize(0);
		addr_bo.initialize(0);
		weight.resize(10);
		for(int i(0);i<10;i++)
		{
			data_co[i].initialize(0);
			weight[i].resize(49);
		}
		out_hidden.resize(10);
		test.resize(49);
		SC_CTHREAD(weight_read, clk_i.pos());
		SC_METHOD(input_read, clk_i.pos());
		sensitive << clk_i.pos();

	}

private:
	vector<vector<float>> weight;
	vector<float> out_hidden;
	vector<float> test;
};

int core::count = 0;


