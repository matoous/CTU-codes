/*
 * File name: Lab06.java
 * Date:      2014/08/26 21:39
 * Author:    @matoous
 */

package cz.cvut.fel.pjv;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.Scanner;

public class Lab02 { 
   public void start(String[] args) throws IOException {
	TextIO tio = new TextIO();
        if(args.length == 0 || (args.length > 0 && (!"1".equals(args[0]) && !"2".equals(args[0]) && !"3".equals(args[0]) && !"4".equals(args[0]) ) ) ){
            homework(tio);
         }
   }
   
   private void printTheLine(double[] nums, int num_c){
                    double avg = 0;
                    for (int j = 0; j < num_c; j++) avg += nums[j];
                    avg /= num_c;
                    double o = 0;
                    for (int j = 0; j < num_c; j++) o += (avg - nums[j])*(avg - nums[j]);
                    o /= num_c;
                    double dev = Math.sqrt(o);
                    System.out.printf("%2d %.3f %.3f\n", num_c, avg, dev);
   }
   
   private void homework(TextIO tio) throws IOException{
        int num_c = 0;
        double[] nums = new double[10];
        double num;   
        String linn;
        int i = 0;
        while(!"".equals(linn = tio.getLine())){
            i++;
            if(TextIO.isDouble(linn)) nums[num_c++] = Double.parseDouble(linn);
            else System.err.printf("A number has not been parsed from line %d\n", i);
            if(num_c == 10){
                printTheLine(nums, num_c);
                num_c = 0;
            }
        }
        System.err.println("End of input detected!");
        if(num_c > 1) printTheLine(nums, num_c);
   }
}
