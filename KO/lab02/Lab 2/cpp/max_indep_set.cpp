#include <gurobi_c++.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
	// data -------------------------------------------------------
	int num_vertices = 5;
	vector<pair<int, int>> edges = { {0,1}, {2,3}, {0,4}, {3,1}, {3,4} };  

	// model ------------------------------------------------------
	GRBEnv env;  // create new environment
    GRBModel model(env); // create empty optimization model

	// - add variables
	GRBVar x[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		x[i] = model.addVar(0, 1, 0.0, GRB_BINARY, "x" + i);
	}
	
	model.update(); // integrate new variables into model
	
	// - add constraints
	for (auto &edge: edges) {
		model.addConstr(x[edge.first] + x[edge.second] <= 1);
    }
	
	// - set objective
    GRBLinExpr obj = 0;
    for (int i = 0; i < num_vertices; i++) {
      obj += x[i];
    }
    model.setObjective(obj, GRB_MAXIMIZE);  

	// call the solver ---------------------------------------------
	model.optimize();

	// print the solution ------------------------------------------
	if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
		cout << "Optimal objective value: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		for (int i = 0; i < num_vertices; i++) {
			cout << "Vertex " << i << " in the maximum independent set? " << x[i].get(GRB_DoubleAttr_X) << endl;
		}
	} else {
		cout << "Optimal solution was not found." << endl;
	}  

  return 0;
}
