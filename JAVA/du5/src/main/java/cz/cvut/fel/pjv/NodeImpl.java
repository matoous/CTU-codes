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
public class NodeImpl implements Node{

    // setters would be better
    public Node leftn;
    public Node rightn;
    private int value;
    
    @Override
    public Node getLeft() {
        return this.leftn;
    }

    @Override
    public Node getRight() {
        return this.rightn;
    }

    @Override
    public int getValue() {
        return this.value;
    }
    
    public NodeImpl(int value){
        this.value = value;
    }
    
}
