/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cz.cvut.fel.pjv;

/**
 *
 * @author matoous
 */
public class TreeImpl implements Tree{
        
        Node root;
        int[] values;
        
        public Node buildTree(int s, int e){
            if(s == e) return new NodeImpl(this.values[s]);
            else{
                int mid = (e-s+1)/2 + s;
                NodeImpl n = new NodeImpl(this.values[mid]);
                n.leftn = buildTree(s,mid-1);
                n.rightn = (mid+1 <= e) ? (buildTree(mid+1,e)) : null;
                return n;
            }
        }
        
        /**
	 * Fills the tree with values from given array.
	 * 
	 * @param values new values of the tree
	 */
        @Override
	public void setTree(int[] values){
            this.values = values;
            this.root = (values.length > 0) ? buildTree(0, values.length - 1) : null;
        }
	
	/**
	 * @return root of the tree, or null if the tree is empty.
	 */
        @Override
	public Node getRoot(){
            return this.root;
        }
	
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
        @Override
	public String toString(){
            String product = "";
            if(this.root == null) return product;
            else{
                product += "- " + Integer.toString(this.root.getValue()) + "\n";
                if(this.root.getLeft() != null) product += printTree(this.root.getLeft(), 1);
                if(this.root.getRight() != null) product += printTree(this.root.getRight(), 1);
            }
            return product;
        }
        
        private String printTree(Node n, int depth){
            String product = "";
            for (int i = 0; i < depth; i++) {
                product += " ";
            }
            product += "- " + Integer.toString(n.getValue()) + "\n";
            if(n.getLeft() != null) product += printTree(n.getLeft(), depth+1);
            if(n.getRight() != null) product += printTree(n.getRight(), depth+1);
            return product;
        }
}
