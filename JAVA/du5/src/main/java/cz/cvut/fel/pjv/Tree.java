package cz.cvut.fel.pjv;

/**
 * Interface Tree represents binary tree.
 */
public interface Tree {
	
	/**
	 * Fills the tree with values from given array.
	 * 
	 * @param values new values of the tree
	 */
	public void setTree(int[] values);
	
	/**
	 * @return root of the tree, or null if the tree is empty.
	 */
	public Node getRoot();
	
	/**
	 * Tree description looks as follows (for tree created from values [1, 2, 3, 4, 5, 6, 7]):
	 * - 4
	 *  - 2
	 *   - 1
	 *   - 3
	 *  - 6
	 *   - 5
	 *   - 7
	 * 
	 * @return String representation of the tree, or "" for empty tree
	 */
	public String toString();
}
