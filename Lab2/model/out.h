#pragma once

#include "systemc.h"

using namespace std;

SC_MODULE(out_module) {

	sc_in<bool>  clk_i;
	sc_out<int>   addr_bo;		// reading address
	sc_in<int>   data_bi;		// reading data
	sc_out<bool>  rd_o;		// reading out flag
	sc_in<bool>  wr_i;					// writing in flag
	sc_out<int>  data_bo;				// writing data
	 
	// get out array
	vector<int> get_out() {
		return buff;
	}

	// parsing result
	void parsing_result(vector<int> out) {
		if (out[0] == 1 && out[1] == 0 && out[2] == 0)
			cout << "CIRCLE";

		else if (out[0] == 0 && out[1] == 1 && out[2] == 0)
			cout << "SQUARE";

		else if (out[0] == 0 && out[1] == 0 && out[2] == 1)
			cout << "TRIANGLE";

		else 
			cout << "CAN'T RECOGNIZE";
	}

	// arrays comparison
	//bool is_equal(vector<int> v1, vector<int> v2) {
	//	if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2])
	//		return true;
	//	else
	//		return false;
	//}

	//out main thread
	void give_me_out()
	{
		while (!wr_i.read()) { wait(); }
		for (int i(0); i < ideal_dim; i++) {
			if (wr_i.read()) {
				//cout << "@" << sc_time_stamp() << endl;
				buff[i] = data_bi.read();
			}
			wait();
		}

		//for (int i(0); i < 3; i++) {
		//	arr1[i] = buff[i];
		//	arr2[i] = buff[i + 3];
		//	arr3[i] = buff[i + 6];
		//}

		//if (is_equal(arr1, arr2) || is_equal(arr1, arr3))
		//	out_arr = arr1;

		//if (is_equal(arr2, arr3))
		//	out_arr = arr2;
	
		//for (int i(0); i < ideal_dim; i++) {
		//	data_bo.write(buff[i]);
		//	wait();
		//}

		//data_bo.write(0);
	
	}

	SC_CTOR(out_module) {

		addr_bo.initialize(0);
		buff.resize(ideal_dim);
		//arr2.resize(3);
		//arr3.resize(3);
		//out_arr.resize(3);

		for (int i(0); i < ideal_dim; i++)
			buff[i] = 0;

		SC_CTHREAD(give_me_out, clk_i.pos());

	}

private:
	vector<int> buff;
	//vector<int> arr1, arr2, arr3, out_arr;
};