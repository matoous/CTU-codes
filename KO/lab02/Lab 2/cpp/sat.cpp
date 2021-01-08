#include <gurobi_c++.h>  // include Gurobi lib
using namespace std;

int main(int argc, char *argv[]) {
    // data -------------------------------------------------------
	// SAT formula: (x0 or ~x1) and (~x0 or x1 or x2)
	int n_vars = 3;
	
	// model ------------------------------------------------------
	GRBEnv env;  // create new environment
    GRBModel model(env); // create empty optimization model

	// - add variables    	
	// addVars(int count, char type=GRB_CONTINUOUS)
    GRBVar* x = model.addVars(n_vars, GRB_BINARY);

    model.update(); // integrate new variables into model
	
	// - add constraints
    model.addConstr(x[0] + (1-x[1]) >= 1);
    model.addConstr((1-x[0]) + x[1] + x[2] >= 1);

	// - set objective
    // we look for any feasible solution, objective is not necessary

	// call the solver ---------------------------------------------
    model.optimize();

	// print the solution ------------------------------------------
    if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {      
      cout << "x0: " << x[0].get(GRB_DoubleAttr_X) << endl;
	  cout << "x1: " << x[1].get(GRB_DoubleAttr_X) << endl;
	  cout << "x2: " << x[2].get(GRB_DoubleAttr_X) << endl;      
    } else {
		cout << "Optimal solution was not found." << endl;
	}

    return 0;
}
