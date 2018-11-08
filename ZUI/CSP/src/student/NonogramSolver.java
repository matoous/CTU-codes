/*
  Created by dzivjmat on 31/03/2018.
 */

package student;

import java.util.*;

public class NonogramSolver {
    private int numOfRows;
    private int numOfColumns;

    private ArrayList<Constraint>[] rowsConstraints;
    private ArrayList<Constraint>[] columnsConstraints;
    private ArrayList[][] sortedMatrix;

    private boolean firstTime = true;

    /**
     * Puzzle solver for nonogram, using backtracking and ac3
     * Prints result to stdout
     *
     * @param in input
     */
    NonogramSolver(Input in) {
        // set variables
        this.numOfRows = in.rows;
        this.numOfColumns = in.cols;
        this.rowsConstraints = in.rowConstraints;
        this.columnsConstraints = in.columnConstraints;
        this.sortedMatrix = new ArrayList[numOfRows][numOfColumns];

        // preprocessing
        for (int row = 0; row < numOfRows; row++) {
            for (int column = 0; column < numOfColumns; column++) {

                // get domain for given position [_, _]
                HashMap<Character, Integer> tmpMap = new HashMap<>();
                for (Map.Entry<Character, Integer> rowEntry : in.rowsDomainMapArr[row].entrySet()) {
                    char key = rowEntry.getKey();
                    int val = rowEntry.getValue();
                    if (in.columnsDomainMapArr[column].containsKey(key)) {
                        tmpMap.put(key, val + in.columnsDomainMapArr[column].get(key));
                    }
                }

                // sort them based on count
                sortedMatrix[row][column] = new ArrayList<>(tmpMap.keySet());
                Collections.sort(sortedMatrix[row][column], (Comparator<Character>) (o1, o2) -> tmpMap.get(o2) - tmpMap.get(o1));
            }
        }
    }

    /**
     * Backtracking wrapper
     */
    void findSolution() {
        backtrackingSearch(new Position(0, 0), new char[numOfRows][numOfColumns], sortedMatrix[0][0]);
        if (firstTime) {
            System.out.println("null");
        }
    }

    /**
     * Backtracking csp algorithm
     */
    private char[][] backtrackingSearch(Position position, char[][] image, ArrayList<Character> var_domains) {
        if (assignmentIsComplete(position)) return image;

        //Least-constraining-value heuristic--> in our case most mentioned values for current row and column (including empty char)
        for (char domain : ac3(position, image, var_domains)) {

            image[position.x][position.y] = domain;
            Position newPosition = (position.y == numOfColumns - 1) ?
                    new Position(position.x + 1, 0) : new Position(position.x, position.y + 1);

            ArrayList<Character> forward_checked_domains = newPosition.x == numOfRows ? null : forwardChecking(newPosition, image, sortedMatrix[newPosition.x][newPosition.y]);
            char[][] result = backtrackingSearch(newPosition, image, forward_checked_domains);

            if (assignmentIsComplete(newPosition)) {
                // print new line on more results
                if (firstTime) firstTime = false;
                else System.out.println();

                assert result != null;
                Arrays.stream(result).forEach(row -> {
                    System.out.println(String.valueOf(row));
                });


            }
            else if (result != null)
                return result; // we are done and we have result, return it
        }
        // no solution found
        return null;
    }

    /**
     * Arc Consistency Algorithm #3
     *
     * @param position current position on image
     * @param assignment current configuration of image
     * @param var_domains domains for current position
     * @return possible assignments for given position
     */
    private List<Character> ac3(Position position, char[][] assignment, ArrayList<Character> var_domains) {
        List<Character> result = new ArrayList<>();
        LinkedList<Character> Q = new LinkedList<>();
        Q.addAll(var_domains);
        while (!Q.isEmpty()){
            char c = Q.poll();
            assignment[position.x] [position.y] = c;
            if (isStateValid(assignment, position)) result.add(c);
        }
        return result;
    }

    /**
     * Check if given image is valid
     * @param assignment current state of image
     * @param position position in image
     * @return true if state is valid, false otherwise
     */
    private boolean isStateValid(char[][] assignment, Position position) {
        return checkRowConstraints(assignment, position) && checkColumnConstraints(assignment, position);
    }

    /**
     * Check column constraints validity
     * @param assignment current image configuration
     * @param position current position
     * @return true if state is valid, false otherwise
     */
    private boolean checkColumnConstraints(char[][] assignment, Position position) {
        int currentRow = 0;
        int currentColumn = position.y;

        for (Constraint constraint : columnsConstraints[position.y]) {
            // check each constraint in given column
            boolean ok = false;
            int currCounter = 0;
            while ( currentRow <= position.x ) { // check from TOP to current ROW
                if (assignment[currentRow][currentColumn] == constraint.color) {
                    // check current constraint usage
                    currCounter++;
                    if (currCounter > constraint.count) return false;
                    if (currCounter == constraint.count) ok = true;
                }
                else if (currCounter == constraint.count) {
                    ok = true;
                    break;
                }
                else if (currCounter != 0 || assignment[currentRow][currentColumn] != '_') return false;

                currentRow++;
            }
            if ((position.x == numOfRows - 1) && !ok) {
                return false;
            }
        }

        // check that rest of the column is empty
        for (; currentRow <= position.x; currentRow++) {
            if (assignment[currentRow][currentColumn] != '_') return false;
        }

        return true;
    }

    /**
     * Check row constraints
     * @param assignment current image configuration
     * @param position current position
     * @return true if state is valid, false otherwise
     */
    private boolean checkRowConstraints(char[][] assignment, Position position) {
        int currentRow = position.x;
        int currCounter = 0;

        for (Constraint constraint : rowsConstraints[position.x]) {
            boolean ok = false;
            int keyCounter = 0;

            while ( currCounter <= position.y) {
                if (assignment[currentRow][currCounter] == constraint.color) {
                    keyCounter++;
                    if (keyCounter > constraint.count) return false;
                    if (keyCounter == constraint.count) ok = true;
                }
                else if (keyCounter == constraint.count) {
                    ok = true;
                    break;
                }
                else if (keyCounter != 0 || assignment[currentRow][currCounter] != '_') return false;

                currCounter++;
            }
            if ((position.y == numOfColumns - 1) && !ok) {
                return false;
            }

        }

        // check that rest of the row is empty
        for (; currCounter <= position.y; currCounter++) {
            if (assignment[currentRow][currCounter] != '_') return false;
        }

        return true;
    }

    /**
     * Get possible assignments for given position but without AC3
     * @param position position
     * @param assignment current image configuration
     * @param var_domains possible domains
     * @return list of possible assignments
     */
    private ArrayList<Character> forwardChecking(Position position, char[][] assignment, ArrayList<Character> var_domains) {
        ArrayList<Character> result = new ArrayList<>();
        // if matrix is full
        if (var_domains == null) return null;
        for (char domain : var_domains) {
            assignment[position.x][position.y] = domain;
            if (isStateValid(assignment, position)) {
                result.add(domain);
            }
        }
        return result;
    }

    /**
     * Assignment is complete if we have finished all rows
     * @param position position to check
     * @return true if we are done, false otherwise
     */
    private boolean assignmentIsComplete(Position position) {
        return (position.x == numOfRows) && (position.y == 0);
    }
}
