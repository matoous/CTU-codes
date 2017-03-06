/*
 * File name: TextIO.java
 * Date:      2014/08/26 21:45
 * Author:    @author
 */

package cz.cvut.fel.pjv;

import java.util.Scanner;

public class TextIO {

   public static boolean isInteger(String s) {
      boolean ret = true;
      try { 
	 Integer.parseInt(s); 
      } catch(NumberFormatException e) { 
	 ret = false; 

      }
      return ret;
   }

   public static boolean isFloat(String s) {
      boolean ret = true;
      try { 
	 Float.parseFloat(s); 
      } catch(NumberFormatException e) { 
	 ret = false; 
      }
      return ret;
   }

   public static boolean isDouble(String s) {
      boolean ret = true;
      try { 
	 Double.parseDouble(s); 
      } catch(NumberFormatException e) { 
	 ret = false; 
      }
      return ret;
   }

   public String getLine() {
      return stdin.hasNext() ? stdin.nextLine() : "";
   }

   public TextIO() {
      stdin = new Scanner(System.in);
   }
   Scanner stdin;
} 

/* end of TextIO.java */
