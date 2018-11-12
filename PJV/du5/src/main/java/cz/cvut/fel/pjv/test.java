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
public class test {
    public static void main(String[] args) {
        TreeImpl ti = new TreeImpl();
        int[] arr = {1,2,3,4,5,6,7,8,9,10};
        ti.setTree(arr);
        System.out.println(ti.toString());
    }
}
