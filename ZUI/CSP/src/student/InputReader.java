/*
  Created by dzivjmat on 31/03/2018.
 */

package student;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;

class Input {
    public final int rows;
    public final int cols;
    public final ArrayList<Constraint>[] rowConstraints;
    public final ArrayList<Constraint>[] columnConstraints;
    public final HashMap<Character,Integer>[] rowsDomainMapArr;
    public final HashMap<Character,Integer>[] columnsDomainMapArr;

    public Input(int rows, int cols,
                 ArrayList<Constraint>[] rowConstraints,
                 ArrayList<Constraint>[] columnConstraints,
                 HashMap<Character,Integer>[] rowsDomainMapArr,
                 HashMap<Character,Integer>[] columnsDomainMapArr
    ){
        this.rows = rows;
        this.cols = cols;
        this.rowConstraints = rowConstraints;
        this.columnConstraints = columnConstraints;
        this.rowsDomainMapArr = rowsDomainMapArr;
        this.columnsDomainMapArr = columnsDomainMapArr;
    }

    public void Print(){
        System.out.printf("Rows %d:\n", this.rows);
        Arrays.stream(this.rowConstraints).forEach(cs -> {
            cs.forEach(c -> System.out.printf("%s ", c.toString()));
            System.out.println("");
        });
        System.out.printf("Cols %d:\n", this.cols);
        Arrays.stream(this.columnConstraints).forEach(cs -> {
            cs.forEach(c -> System.out.printf("%s ", c.toString()));
            System.out.println("");
        });
    }
}

public class InputReader {
    public static Input Read() throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer st = new StringTokenizer(br.readLine(), ",");

        int rows = Integer.parseInt(st.nextToken());
        int columns = Integer.parseInt(st.nextToken());

        ArrayList<Constraint>[] rowsConstraints = new ArrayList[rows];
        ArrayList<Constraint>[] columnsConstraints = new ArrayList[columns];
        HashMap<Character,Integer>[] rowsDomainMapArr = new HashMap[rows];
        HashMap<Character,Integer>[] columnsDomainMapArr = new HashMap[columns];

        ReadConstraints(br, rows, columns, rowsConstraints, rowsDomainMapArr);

        ReadConstraints(br, columns, rows, columnsConstraints, columnsDomainMapArr);


        return new Input(rows, columns, rowsConstraints, columnsConstraints, rowsDomainMapArr, columnsDomainMapArr);
    }

    private static void ReadConstraints(BufferedReader br,
                                        int rows,
                                        int columns,
                                        ArrayList<Constraint>[] constraints,
                                        HashMap<Character, Integer>[] domainMapArr) throws IOException {
        StringTokenizer st;
        Constraint constraint;

        for (int row = 0; row < rows; row++) {
            ArrayList<Constraint> localConstraints = new ArrayList<>();
            HashMap<Character, Integer> localDomainMap = new HashMap<>();
            // add empty space to domain
            localDomainMap.put('_', columns);

            st = new StringTokenizer(br.readLine(), ",");

            while (st.hasMoreElements()) {
                constraint = new Constraint(st.nextToken().charAt(0), Integer.parseInt(st.nextToken()));
                localConstraints.add(constraint);
                if (localDomainMap.containsKey(constraint.color)){
                    localDomainMap.put(constraint.color,  localDomainMap.get(constraint.color)+1);
                } else {
                    localDomainMap.put(constraint.color, 1);
                }
                localDomainMap.put ('_', localDomainMap.get('_')-1);
            }

            domainMapArr[row] = localDomainMap;
            constraints[row] = localConstraints;
        }
    }
}
