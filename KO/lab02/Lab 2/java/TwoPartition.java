import gurobi.*;
import java.util.stream.*;

public class TwoPartition{
    public static void main(String[] args) throws Exception {
		// data --------------------------------------------------------------		
        int[] values = new int[]{100, 50, 50, 50, 20, 20, 10};
        int nValues = values.length;

		// model -------------------------------------------------------------        
        GRBEnv env = new GRBEnv();  // create new environment        
        GRBModel model = new GRBModel(env);  // create empty optimization model

		// - add variables
        // addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
        GRBVar[] x = new GRBVar[nValues];
        for (int i = 0; i < nValues; i++){
            x[i] =  model.addVar(0.0, 1.0, 0.0, GRB.BINARY, "x" + i);
        }
                
        model.update();  // integrate new variables into model

        // - add constraints
		// addConstr(leftHandSide, inequalityType, rightHandSide, name)
        GRBLinExpr cons = new GRBLinExpr();		
        for (int i = 0; i < nValues; i++) {
            cons.addTerm(values[i], x[i]);
        }
        model.addConstr(cons, GRB.EQUAL, IntStream.of(values).sum() / 2, "cons");

        // call the solver ---------------------------------------------------
        model.optimize();

		// print the solution ------------------------------------------------        
        if (model.get(GRB.IntAttr.Status) == GRB.Status.OPTIMAL) {
			for (int i = 0; i < nValues; i++){
				System.out.println(x[i].get(GRB.StringAttr.VarName) + " " + x[i].get(GRB.DoubleAttr.X));
			}
		} else {
			System.out.println("Optimal solution was not found.");
		}    
    }
}
