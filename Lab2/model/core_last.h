#pragma once
#include "systemc.h"
#include "main.h"
#include "IO.h"

#define epochs 20

SC_MODULE(core_last) {



	sc_in<bool>  clk_i;
	sc_in <float> data_ci[30];
	sc_in<bool>	 wr_ci[3];						// reading input flag
	sc_out <float> data_bo[3];	
	sc_out<bool> rd_bo;						// reading input flag
	sc_out<bool> wr_bo;						// writing output flag
	sc_out<int>  addr_bo;			    	// writing address
	sc_in <float>  data_bi[30];					// weight data


	// print float prediction
	void get_prediction() {
		cout << "Prediction: ";
		for_ideal()
			cout << prediction[i] << " ";
		cout << endl;
	}

	// sending results to the bus 
	void bus_write(int addr, vector<float> data) {
		//addr_bo.write(addr);
		for (int i = 0; i < 3; i++)
		{
			data_bo[i].write(data[i]);
		}
		wr_bo.write(1);
		//wait();
		//wr_bo.write(0);
	}

	// core main method
	void input_read()
	{
		if (wr_ci[0].read() && wr_ci[1].read() && wr_ci[2].read())
		{
			for (int i(0); i < 30; i++)
			{
				data_neuron[i] = (float)data_ci[i].read();
			}
			for (int i(0); i < 3; i++)
			{
				prediction[i] = 0;
				for (int j(0); j < 30; j++)
				{
					prediction[i] += data_neuron[j] * weight[i][j];
				}
				prediction[i] = 1 / (1 + exp(-prediction[i]));
				//cout << prediction[i] << " ";
			}
			bus_write(33, prediction);
		}

	}
	//core main thread
	void weight_read() {
		for (int i = 0; i < 120; i++) wait();
		for (int i(0); i < 3; i++)
		{
			addr_bo.write(i+30);
			rd_bo.write(1);
			wait();
			wait();
			rd_bo.write(0);
			wait();
			wait();
			for (int j = 0; j < 30; j++)
			{
				weight[i][j] = data_bi[j].read();
			}
		}
		//cout << weight[0][0];
	}


	SC_CTOR(core_last) {

		addr_bo.initialize(0);
		rd_bo.initialize(0);
		wr_bo.initialize(0);

		data_neuron.resize(30);
		prediction.resize(3);
		weight.resize(3);
		for (int i(0); i < 3; i++)
		{
			data_bo[i].initialize(0);
			weight[i].resize(49);
		}

		SC_CTHREAD(weight_read, clk_i.pos());
		SC_METHOD(input_read, clk_i.pos());
		sensitive << clk_i.pos();

	}

private:
	vector<vector<float>> weight;
	vector<float> data_neuron;
	vector<float> prediction;
};


