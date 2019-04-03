import javax.swing.JPanel;
import javax.swing.JFrame;
import java.awt.*;
import java.util.*;
import java.io.*;


/** A class prepared for CS435 Project2
    GUI - 2D points and the convex hull
    @author ZHD
 */

public class ConvexHull_GUI extends JPanel 
{ 
   private ArrayList<Point> data2DPoints = new ArrayList<Point>(); 
   private ArrayList<Point> convexHull = new ArrayList<Point>();
   
   public ConvexHull_GUI() {
      this.setPreferredSize(new Dimension(900,600));
   }
  /* 
   public void setData(ArrayList<Point> d) {
      this.data2DPoints = d;
   }
   public void setHull(ArrayList<Point> h) {
      this.data2DPoints = h;
   }
   */
   public ArrayList<Point> getData() {
      return data2DPoints;
   }
   public ArrayList<Point> getHull() {
      return convexHull;
   }
   
   public void readDataFile(String file, char dh) 
   { 
      System.out.println("reading a file ... ... ");
      try { 
         Scanner sc = new Scanner(new File(file)); 
         while (sc.hasNextLine()) 
         { 
            Point aPoint = new Point();
            if (sc.hasNextInt())
               aPoint.x = sc.nextInt(); 
            else
               throw new IOException();
            if (sc.hasNextInt())
               aPoint.y = sc.nextInt();
            else
               throw new IOException();
            if (dh=='d')
               data2DPoints.add(aPoint);
            else
               convexHull.add(aPoint); 
         } 
         sc.close(); 
      } 
      catch (IOException e) { // TODO Auto-generated catch block 
         System.out.println("IO Exception");
         e.printStackTrace(); 
      } 
   }
   
   //find max & min of all points
   public void getMaxMin(ArrayList<Point> d, ArrayList<Point> h, Point[] maxmin){ 
      maxmin[0] = new Point(-1*Integer.MAX_VALUE,-1*Integer.MAX_VALUE); //0 -- max
      maxmin[1] = new Point(Integer.MAX_VALUE, Integer.MAX_VALUE);  //1 -- min 
   
      if (d!=null) {         
         for (int i=0; i<d.size(); i++) {
            if (d.get(i).x<maxmin[1].x)
               maxmin[1].x = d.get(i).x;
            else if (d.get(i).x>maxmin[0].x)
               maxmin[0].x = d.get(i).x;
            if (d.get(i).y<maxmin[1].y)
               maxmin[1].y = d.get(i).y;
            else if (d.get(i).y>maxmin[0].y)
               maxmin[0].y = d.get(i).y;
         } 
      }
      if (h!=null) {
         for (int i=0; i<h.size(); i++) {
            if (h.get(i).x<maxmin[1].x)
               maxmin[1].x = h.get(i).x;
            else if (h.get(i).x>maxmin[0].x)
               maxmin[0].x = h.get(i).x;
            if (h.get(i).y<maxmin[1].y)
               maxmin[1].y = h.get(i).y;
            else if (h.get(i).y>maxmin[0].y)
               maxmin[0].y = h.get(i).y;
         } 
      }
   }
   
   public void paintComponent(Graphics g)
   {
      super.paintComponent(g);
      int r = Math.min(8, Math.min(getWidth(), getHeight())); //radius of a point
      Point[] maxmin = new Point[2];
      
      getMaxMin(data2DPoints, convexHull, maxmin); //max & min are used for scaling
      
      if (data2DPoints!=null) {
         for (int i=0; i<data2DPoints.size(); i++) {
            int x=data2DPoints.get(i).x, y=data2DPoints.get(i).y;
            //scale the location of the point
            if (maxmin[0].x-maxmin[1].x>0)
               x = (data2DPoints.get(i).x-maxmin[1].x)*(getWidth()-2*r)/(maxmin[0].x-maxmin[1].x)+r/2;
            if (maxmin[0].y-maxmin[1].y>0)
               y = (data2DPoints.get(i).y-maxmin[1].y)*(getHeight()-2*r)/(maxmin[0].y-maxmin[1].y)+r/2;   
            g.fillOval(x,y,r,r);
         }
      }
      if (convexHull!=null) {
         g.setColor(Color.PINK);
         //draw the 1st point
         int x0=convexHull.get(0).x, y0=convexHull.get(0).y;
         //scale the location of the point
         if (maxmin[0].x-maxmin[1].x>0)
            x0 = (convexHull.get(0).x-maxmin[1].x)*(getWidth()-2*r)/(maxmin[0].x-maxmin[1].x)+r/2;
         if (maxmin[0].y-maxmin[1].y>0)
            y0 = (convexHull.get(0).y-maxmin[1].y)*(getHeight()-2*r)/(maxmin[0].y-maxmin[1].y)+r/2;  
         g.drawOval(x0,y0,r,r);
         
         int x1=x0, y1=y0;
         for (int i=1; i<convexHull.size(); i++) {
            int x=convexHull.get(i).x, y=convexHull.get(i).y;
            //scale the location of the point
            if (maxmin[0].x-maxmin[1].x>0)
               x = (convexHull.get(i).x-maxmin[1].x)*(getWidth()-2*r)/(maxmin[0].x-maxmin[1].x)+r/2;
            if (maxmin[0].y-maxmin[1].y>0)
               y = (convexHull.get(i).y-maxmin[1].y)*(getHeight()-2*r)/(maxmin[0].y-maxmin[1].y)+r/2;   
            
            g.drawOval(x,y,r,r); 
            g.drawLine(x1+r/2, y1+r/2, x+r/2, y+r/2);
            x1=x; y1=y;
         }
         g.drawLine(x0+r/2, y0+r/2, x1+r/2, y1+r/2);
      }
   }
   public void testFileGen(String testFileName, String type, int nPoints) {
      int width = 100;
      Point p = new Point();
      Random rand = new Random(); 
      int range = 600;
      
      System.out.println("writing a file ... ... " + nPoints +" points.");
      try { 
         PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(testFileName)));
      
         if (type.equals("circle")) { //points within a circle
            for (int i=0; i<nPoints; i++) {
               //get a point on a circle of radius range/2 and centered at (0,0)
               int x = rand.nextInt(range)-range/2;                
               int y = (int) ((2*(rand.nextInt(2)-0.5))*Math.sqrt((range*range/4)-x*x));
               x = (int) (x*(rand.nextInt(range)/(double) range));
               y = (int) (y*(rand.nextInt(range)/(double) range));
               //shift to make the center at (range/2, range/2) 
               x = x+range/2;
               y = y+range/2;
               if (i<nPoints-1)
                  out.write(x+"\t"+y+"\n");
               else
                  out.write(x+"\t"+y);
            }
         }
         else if (type.equals("onCircle")) {
            for (int i=0; i<nPoints; i++) {
               //get a point on a circle of radius range/2 and centered at (0,0)
               int x = rand.nextInt(range)-range/2;                
               int y = (int) ((2*(rand.nextInt(2)-0.5))*Math.sqrt((range*range/4)-x*x));
               //shift to make the center at (range/2, range/2) 
               x = x+range/2;
               y = y+range/2;
               if (i<nPoints-1)
                  out.write(x+"\t"+y+"\n");
               else
                  out.write(x+"\t"+y);
            }
         }
         else if (type.equals("rectangle")) { // within a rectangle
            for (int i=0; i<nPoints; i++) {
               int x = rand.nextInt(range);                
               int y = rand.nextInt(range);
               
               if (i<nPoints-1)
                  out.write(x+"\t"+y+"\n");
               else
                  out.write(x+"\t"+y);
            }
         }
         else if (type.equals("onRectangle")) { //points within a rangexrange rectangle
            nPoints = nPoints/4*4; //ensure nPoints%4==0
            int n = 0;
            for (int j=0; j<4;j++) {
               for (int i=0; i<nPoints/4; i++) {
                  int x, y;
                  if (j==0) {
                     x = 0;                
                     y = rand.nextInt(range);
                  }
                  else if (j==1) {
                     x = rand.nextInt(range);
                     y = 0;                
                  }
                  else if (j==2) {
                     x = range;
                     y = rand.nextInt(range);                
                  }
                  else {
                     x = rand.nextInt(range);
                     y = range;                
                  }
                  n++;
                  if (n<nPoints)
                     out.write(x+"\t"+y+"\n");
                  else
                     out.write(x+"\t"+y);
               }
            }
         }
         else if (type.equals("triangle")) { //points in an equilateral triangle
            for (int i=0; i<nPoints; i++) {
               int y = (int) (rand.nextInt(range)*Math.sqrt(3)/2); 
               int bound = Math.max(1, (int)(range/2 - y/Math.sqrt(3)));
               int x = (int) (2*(rand.nextInt(2)-0.5)*rand.nextInt(bound));                
               
               //shift to make the center of the base at (0, range/2) 
               x = x+range/2;
               y = (int) (range*Math.sqrt(3)/2-y);
               if (i<nPoints-1)
                  out.write(x+"\t"+y+"\n");
               else
                  out.write(x+"\t"+y);
            }
         }
         else if (type.equals("onTriangle")) { //points in an equilateral triangle
            for (int i=0; i<nPoints; i++) {
               int y = (int) (rand.nextInt(range)*Math.sqrt(3)/2); 
               int x = (int) (2*(rand.nextInt(2)-0.5)*(range/2 - y/Math.sqrt(3)));                
               
               //shift to make the center of the base at (0, range/2) 
               x = x+range/2;
               y = (int) (range*Math.sqrt(3)/2-y);
               if (i<nPoints-1)
                  out.write(x+"\t"+y+"\n");
               else
                  out.write(x+"\t"+y);
            }
         }
         else { //to be implemented
            
         }
         out.close();
      }
      catch (IOException e) { // TODO Auto-generated catch block 
         System.out.println("IO Exception");
         e.printStackTrace(); 
      }
   }
   
   public static void main(String[] args) {
      ConvexHull_GUI CH = new ConvexHull_GUI();
      boolean generateData = false;
      if (generateData){
         CH.testFileGen("test.txt", "triangle", 1000); //generate data points
         System.out.println("done");
         System.exit(0);
      }
      else {
         //visulizing the points and convex hull      
         String dataFile = "test.txt";
         String hullFile = "hull.txt";
      
         CH.readDataFile(dataFile, 'd');
         System.out.println(dataFile + " has "+CH.getData().size()+" points.");
         CH.readDataFile(hullFile, 'h');
         System.out.println(hullFile + " has "+CH.getHull().size()+" points.");
      
         JFrame jf = new JFrame("Convex Hull");
         jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
         jf.add(CH);
         jf.pack();
         jf.setVisible(true);
      }
   } 
}