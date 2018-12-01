/*
  Created by dzivjmat on 31/03/2018.
 */

package student;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import static student.InputReader.Read;

public class CSPMain {


    // I am using backtracking csp algorithm with ac3
    // Constraints are represented as arrays of individual constraint for given row/column
    // We generate the image from left to right from top to bottom
    //
    //  -> -> -> ->
    //  -> -> __ __
    //  __ __ __ __
    //  __ __ __ __
    //
    public static void main(String[] args) throws IOException {
        Input in = Read();

        NonogramSolver solver = new NonogramSolver(in);

        solver.findSolution();

    }

}
