import gurobi.*;
import java.util.stream.*;

public class SAT {
    public static void main(String[] args) throws Exception {
        // data ----------------------------------------------------
		// SAT formula: (x0 or ~x1) and (~x0 or x1 or x2)
		int nVars = 3;

		// model ---------------------------------------------------        
        GRBEnv env = new GRBEnv();  // create new environment        
        GRBModel model = new GRBModel(env); // create empty optimization model

		// - add variables
        // addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
        GRBVar[] x = new GRBVar[nVars];
        for (int i = 0; i < nVars; i++){
            x[i] =  model.addVar(0.0, 1.0, 0.0, GRB.BINARY, "x" + i);
        }
        
        model.update();  // integrate new variables into model

        // - add constraints
        // addConstr(leftHandSide, inequalityType, rightHandSide, name)			
	    
		// clause1: (x0 or ~x1)
		//  rewritten to (x[0] + (1-x[1])) >= 1, which is (x[0] - x[1] + 1) >= 1
		GRBLinExpr c1 = new GRBLinExpr();	    
		c1.addTerm(1.0, x[0]);
		c1.addTerm(-1.0, x[1]);
		c1.addConstant(1.0);
		model.addConstr(c1, GRB.GREATER_EQUAL, 1, "c1");
		
		// caluse2: (~x0 or x1 or x2)
		GRBLinExpr c2 = new GRBLinExpr();	    
		c2.addTerm(-1.0, x[0]);
		c2.addConstant(1.0);
		c2.addTerm(1.0, x[1]);
		c2.addTerm(1.0, x[2]);	
		model.addConstr(c2, GRB.GREATER_EQUAL, 1, "c2");
		
		// - set objective
		// we want any feasible solution, objective is not necessary

        // call the solver -----------------------------------------
        model.optimize();

		// print the solution --------------------------------------                
        if (model.get(GRB.IntAttr.Status) == GRB.Status.OPTIMAL) {
			for (int i = 0; i < nVars; i++){
				System.out.println(x[i].get(GRB.StringAttr.VarName) + " " + x[i].get(GRB.DoubleAttr.X));
			}
		} else {
			System.out.println("Optimal solution was not found.");
		}
		
    }
}
