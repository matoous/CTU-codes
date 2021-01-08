#include <gurobi_c++.h>
#include <vector>
using namespace std;

// Maximum independent set.

int main(int argc, char *argv[]) {
	// data -------------------------------------------------------
	vector<int> nums = {100, 50, 50, 50, 20, 20, 10};
	int n_nums = nums.size();
	
	// model ------------------------------------------------------
	GRBEnv env;  // create new environment
    GRBModel model(env); // create empty optimization model

	// - add variables
	// addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
	GRBVar x[n_nums];
	for (int i = 0; i < n_nums; i++) {
		x[i] = model.addVar(0, 1, 0.0, GRB_BINARY, "x" + i);
	}
	
	model.update(); // integrate new variables into model

	// - add constraints
	GRBLinExpr e1 = 0;
	GRBLinExpr e2 = 0;
	for (int i = 0; i < n_nums; i++) {
		e1 += x[i] * nums[i];
		e2 += (1-x[i]) * nums[i];
	}
	model.addConstr(e1 == e2);
	
	// - set objective
	// we look for any feasible solution, objective is not necessary
	
	// call the solver ---------------------------------------------
    model.optimize();

	// print the solution ------------------------------------------
    if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {      
      for (int i = 0; i < n_nums; i++) {
		cout << "x" << i << " " << x[i].get(GRB_DoubleAttr_X) << endl;
	  }
    } else {
		cout << "Optimal solution was not found." << endl;
	}
  

  return 0;
}
