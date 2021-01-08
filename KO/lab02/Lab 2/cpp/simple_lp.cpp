#include <gurobi_c++.h>  // include Gurobi lib
using namespace std;

int main(int argc, char *argv[]) {
    // data -------------------------------------------------------
	//  minimize -x + 2y s.t.
	//     -4x - 9y <= -18
	//  (3/2)x -  y <= 27/4
	// (8/17)x -  y >= -2 
	
	// model ------------------------------------------------------
	GRBEnv env;  // create new environment
    GRBModel model(env); // create empty optimization model

	// - add variables    
	// addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
    GRBVar x = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x");
    GRBVar y = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "y");

    model.update(); // integrate new variables into model
	
	// - add constraints
    model.addConstr(-4*x - 9*y <= -18);
    model.addConstr((3.0/2.0)*x - y <= 27.0/4.0);
    model.addConstr((8.0/17.0)*x - y >= -2);

	// - set objective
    model.setObjective(-x + 2*y, GRB_MINIMIZE);

	// call the solver ---------------------------------------------
    model.optimize();

	// print the solution ------------------------------------------
    if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
      cout << "Optimal objective value: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
      cout << "x: " << x.get(GRB_DoubleAttr_X) << " ";
      cout << "y: " << y.get(GRB_DoubleAttr_X) << endl;
    } else {
		cout << "Optimal solution was not found." << endl;
	}

    return 0;
}