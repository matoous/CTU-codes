import gurobi.*;

public class SimpleLp {
	public static void main(String[] args) throws Exception {
		// data --------------------------------------------------------------------
		//  minimize -x + 2y s.t.
		//     -4x - 9y <= -18
		//  (3/2)x -  y <= 27/4
		// (8/17)x -  y >= -2 
     
		// model -------------------------------------------------------------------		
		GRBEnv env = new GRBEnv();  // create new environment
		GRBModel model = new GRBModel(env);  // create empty optimization model

		// - add variables		 
		// addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
		GRBVar x = model.addVar(-GRB.INFINITY, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "x");
		GRBVar y = model.addVar(-GRB.INFINITY, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "y");
		 
		model.update();  // integrate new variables into model

		// - add constraints
		// addConstr(leftHandSide, inequalityType, rightHandSide, name)
		
		// Add constraint: -4x - 9y <= -18
		GRBLinExpr cons1 = new GRBLinExpr();
		cons1.addTerm(-4.0, x);
     	cons1.addTerm(-9.0, y);
		model.addConstr(cons1, GRB.LESS_EQUAL, -18.0, "cons1");
		
		// Add constraint: 3/2x - y <= 27/4
		GRBLinExpr cons2 = new GRBLinExpr();
		cons2.addTerm(3.0 / 2.0, x);
     	cons2.addTerm(-1.0, y);
		model.addConstr(cons2, GRB.LESS_EQUAL, 27.0/4.0, "cons2");

		// Add constraint: 8/17x - y >= 2
		GRBLinExpr cons3 = new GRBLinExpr();
		cons3.addTerm(8.0/17.0, x);
     	cons3.addTerm(-1.0, y);
		model.addConstr(cons3, GRB.GREATER_EQUAL, -2.0, "cons3");
     	
		// set objective: minimize -x + 2y -------------------------------------------
		GRBLinExpr obj = new GRBLinExpr();
		obj.addTerm(-1.0, x);
     	obj.addTerm(2.0, y);
		model.setObjective(obj, GRB.MINIMIZE);

		// call the solver -----------------------------------------------------------
		model.optimize();

		// print the objective -------------------------------------------------------     
		if (model.get(GRB.IntAttr.Status) == GRB.Status.OPTIMAL) {
			System.out.println(x.get(GRB.StringAttr.VarName) + " " + x.get(GRB.DoubleAttr.X));
			System.out.println(y.get(GRB.StringAttr.VarName) + " " + y.get(GRB.DoubleAttr.X));
			System.out.println("Objective: " + model.get(GRB.DoubleAttr.ObjVal));
		} else {
			System.out.println("Optimal solution was not found.");
		}
	}
}
