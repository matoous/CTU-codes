import gurobi.*;
import java.util.stream.*;

public class MinVertexCover {
    public static void main(String[] args) throws Exception {
         // data -----------------------------------------------------------
		int edges[][]={{0, 1},{2,3},{0,4},{3,1},{3,4}};
        int nVertices = 5;
		
		// model ----------------------------------------------------------        
        GRBEnv env = new GRBEnv();  // create new environment        
        GRBModel model = new GRBModel(env);  // create empty optimization model

		// - add variables        
        // addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
        GRBVar[] x = new GRBVar[nVertices];
        for (int i = 0; i < nVertices; i++){
            x[i] =  model.addVar(0.0, 1.0, 0.0, GRB.BINARY, "x" + i);
        }
        
        model.update();  // integrate new variables into model

        // - add constraints
        // addConstr(leftHandSide, inequalityType, rightHandSide, name)	
		for (int i = 0; i < edges.length; i++) {
			GRBLinExpr cons = new GRBLinExpr();
            cons.addTerm(1.0, x[edges[i][0]]);
			cons.addTerm(1.0, x[edges[i][1]]);
			model.addConstr(cons, GRB.GREATER_EQUAL, 1, "cons" + i);
        }
		
		// - set objective
		GRBLinExpr obj = new GRBLinExpr();
		double[] coeffs = new double [nVertices];
        for (int i = 0; i < nVertices; i++) {
            coeffs[i] = 1.0;
        }
        obj.addTerms(coeffs, x);
        model.setObjective(obj, GRB.MINIMIZE);

        // call the solver ------------------------------------------------
        model.optimize();

        // print the solution ---------------------------------------------        
        if (model.get(GRB.IntAttr.Status) == GRB.Status.OPTIMAL) {
			for (int i = 0; i < nVertices; i++){
				System.out.println(x[i].get(GRB.StringAttr.VarName) + " " + x[i].get(GRB.DoubleAttr.X));
			}
		} else {
			System.out.println("Optimal solution was not found.");
		}
    }
}
