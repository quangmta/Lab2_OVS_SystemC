#pragma once

#include "systemc.h"
#include <vector>
//#include "neural_net/Process.h"

#define train_dim 49
#define ideal_dim 3
#define data_dim 52
#define set_size 3

#define for_data() for(int i(0); i < data_dim; i++)
#define for_train() for(int i(0); i < train_dim; i++)
#define for_ideal() for(int i(0); i < ideal_dim; i++)

using namespace std;

SC_MODULE(IO) {

	sc_in<bool>  clk_i;
	sc_out <float> data_o[49];
	sc_in <bool> wr_i;
	sc_in<float> data_i[3];
	sc_out <bool> wr_o;					

	vector<float> test_arr;
	vector<float> prediction;

	vector<int> convert(vector<float> out) {
		vector<int> int_out(out.size());
		for (int i(0); i < int_out.size(); i++) {
			if (out[i] > 0.90)
				int_out[i] = 1;
			else
				int_out[i] = 0;
		}

		return int_out;
	}

	void parsing_result(vector<int> out) {
		if (out[0] == 1 && out[1] == 0 && out[2] == 0)
			cout << "CIRCLE";

		else if (out[0] == 0 && out[1] == 1 && out[2] == 0)
			cout << "SQUARE";

		else if (out[0] == 0 && out[1] == 0 && out[2] == 1)
			cout << "TRIANGLE";

		else
			cout << "CAN'T RECOGNIZE";
		cout << endl;
	}
	void send_data(string file_name)
	{		
		ifstream fin(file_name);
		while (!fin.eof()) {
			for (int i(0); i < train_dim; i++)
				fin >> test_arr[i];
		}

		for_train()
		{
			data_o[i].write(test_arr[i]);
		}
	}
	void result_predict()
	{
		while (!wr_i.read()) wait();
		for (int i(0); i < 3; i++)
		{
			prediction[i] = data_i[i].read();
		}
		for (int i = 0; i < 3; i++)
		{
			cout << prediction[i] << " ";
		}
		cout << endl;
		parsing_result(convert(prediction));
	}

	// input main thread
	void core_write() 
	{
		for (int i = 0; i < 140; i++) wait();
		//SQUARE
		send_data("data/test_square.txt");
		cout << endl;
		wr_o.write(1);
		wait();
		cout << sc_time_stamp() << "  SQUARE"<<endl;
		//CIRCLE
		send_data("data/test_circle.txt");
		wait();
		cout << sc_time_stamp() <<"  CIRCLE"<< endl;
		//TRIANGLE
		send_data("data/test_triangle.txt");
		wait();
		cout << sc_time_stamp() <<"  TRIANGLE"<< endl;
		wr_o.write(0);
		wait();
		cout << endl << "PREDICTION: " << endl;
		cout << endl<<sc_time_stamp() << endl;
		result_predict();		
		wait();
		cout << endl << sc_time_stamp() << endl;
		result_predict();
		wait();
		cout << endl << sc_time_stamp() << endl;
		result_predict();
		wait();
	}

	SC_CTOR(IO) {

		wr_o.initialize(0);

		for_train()
		{
			data_o[i].initialize(0);
		}

		test_arr.resize(train_dim);
		prediction.resize(3);

		SC_CTHREAD(core_write, clk_i.pos());

	}

};


