// You need to complete this program for your second project.

// Standard libraries
#define iPair std::pair<int, int>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <vector>
#include <stack>
#include <set>
#include <ctime>
#include <algorithm>

struct Point {
  int x, y;
};


std::vector<Point> jhull;
std::vector<Point> hull_vector;  
std::set<iPair> hull;

//****GRAHAM SCAN****
// A global point needed for  sorting points with reference 
// to  the first point Used in compare function of qsort() 
Point p0; 
  
// A utility function to find next to top in a stack 
Point nextToTop(std::stack<Point> &S) 
{ 
    Point p = S.top(); 
    S.pop(); 
    Point res = S.top(); 
    S.push(p); 
    return res; 
} 
  
// A utility function to swap two points 
void swap(Point &p1, Point &p2) 
{ 
    Point temp = p1; 
    p1 = p2; 
    p2 = temp; 
} 

// A utility function to return square of distance 
// between p1 and p2 
int distSq(Point p1, Point p2) 
{ 
    return (p1.x - p2.x)*(p1.x - p2.x) + 
          (p1.y - p2.y)*(p1.y - p2.y); 
} 
  
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

// A function used by library function qsort() to sort an array of 
// points with respect to the first point 
int compare(const void *vp1, const void *vp2) 
{ 
   Point *p1 = (Point *)vp1; 
   Point *p2 = (Point *)vp2; 
  
   // Find orientation 
   int o = orientation(p0, *p1, *p2); 
   if (o == 0) 
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1; 
  
   return (o == 2)? -1: 1; 
} 
  
// Prints convex hull of a set of n points. 
void grahamScanConvexHull(std::vector<Point> points, int n, std::string outputDataFile) 
{ 
   // Find the bottommost point 
   int ymin = points[0].y, min = 0; 
   for (int i = 1; i < n; i++) 
   { 
     int y = points[i].y; 
  
     // Pick the bottom-most or chose the left 
     // most point in case of tie 
     if ((y < ymin) || (ymin == y && 
         points[i].x < points[min].x)) 
        ymin = points[i].y, min = i; 
   } 
  
   // Place the bottom-most point at first position 
   swap(points[0], points[min]); 
  
   // Sort n-1 points with respect to the first point. 
   // A point p1 comes before p2 in sorted ouput if p2 
   // has larger polar angle (in counterclockwise 
   // direction) than p1 
   p0 = points[0]; 
   qsort(&points[1], n-1, sizeof(Point), compare); 
  
   // If two or more points make same angle with p0, 
   // Remove all but the one that is farthest from p0 
   // Remember that, in above sorting, our criteria was 
   // to keep the farthest point at the end when more than 
   // one points have same angle. 
   int m = 1; // Initialize size of modified array 
   for (int i=1; i<n; i++) 
   { 
       // Keep removing i while angle of i and i+1 is same 
       // with respect to p0 
       while (i < n-1 && orientation(p0, points[i], 
                                    points[i+1]) == 0) 
          i++; 
  
  
       points[m] = points[i]; 
       m++;  // Update size of modified array 
   } 
  
   // If modified array of points has less than 3 points, 
   // convex hull is not possible 
   if (m < 3) return; 
  
   // Create an empty stack and push first three points 
   // to it. 
   std::stack<Point> S; 
   S.push(points[0]); 
   S.push(points[1]); 
   S.push(points[2]); 
  
   // Process remaining n-3 points 
   for (int i = 3; i < m; i++) 
   { 
      // Keep removing top while the angle formed by 
      // points next-to-top, top, and points[i] makes 
      // a non-left turn 
      while (orientation(nextToTop(S), S.top(), points[i]) != 2) 
         S.pop(); 
      S.push(points[i]); 
   } 
  
  // Export hull to file
  std::ofstream dataFile;
  dataFile.open(outputDataFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   // Now stack has the output points, print contents of stack 
   while (!S.empty()) 
   { 
       Point p = S.top(); 
       dataFile << p.x << "\t" << p.y << "\n";       
       S.pop(); 
   } 
   dataFile.close();
}
//****END GRAHAM SCAN****

//****JARVIS MARCH ALGORITHM****

// Prints convex hull of a set of n points. 
void jarvisMarchConvexHull(std::vector<Point> points, int n, std::string outputDataFile) 
{ 
    // There must be at least 3 points 
    if (n < 3) { 
        std::cout << "More then 3 points are needed" << std::endl;
        return;
    }  
  
  
    // Find the leftmost point 
    int l = 0; 
    for (int i = 1; i < n; i++) 
        if (points[i].x < points[l].x) 
            l = i; 
  
    // Start from leftmost point, keep moving counterclockwise 
    // until reach the start point again.  This loop runs O(h) 
    // times where h is number of points in result or output. 
    int p = l, q; 
    do
    { 
        // Add current point to result 
        jhull.push_back(points[p]); 
  
        // Search for a point 'q' such that orientation(p, x, 
        // q) is counterclockwise for all points 'x'. The idea 
        // is to keep track of last visited most counterclock- 
        // wise point in q. If any point 'i' is more counterclock- 
        // wise than q, then update q. 
        q = (p+1)%n; 
        for (int i = 0; i < n; i++) 
        { 
           // If i is more counterclockwise than current q, then 
           // update q 
           if (orientation(points[p], points[i], points[q]) == 2) 
               q = i; 
        } 
  
        // Now q is the most counterclockwise with respect to p 
        // Set p as q for next iteration, so that q is added to 
        // result 'hull' 
        p = q; 
  
    } while (p != l);  // While we don't come to first point 
  
    std::ofstream dataFile;
    dataFile.open(outputDataFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    // Print Result 
    for (int i = 0; i < hull.size(); i++) 
         dataFile << jhull[i].x << "\t" << jhull[i].y << "\n"; 
    dataFile.close();
} 

//****END JARVIS MARCH ALGORITHM****

//****QUICK HULL****

// Returns the side of point p with respect to line 
// joining points p1 and p2. 
int findSide(iPair p1, iPair p2, iPair p) 
{ 
    int val = (p.second - p1.second) * (p2.first - p1.first) - 
              (p2.second - p1.second) * (p.first - p1.first); 
  
    if (val > 0) 
        return 1; 
    if (val < 0) 
        return -1; 
    return 0; 
} 
  
// returns a value proportional to the distance 
// between the point p and the line joining the 
// points p1 and p2 
int lineDist(iPair p1, iPair p2, iPair p) 
{ 
    return abs ((p.second - p1.second) * (p2.first - p1.first) - 
               (p2.second - p1.second) * (p.first - p1.first)); 
} 

// End points of line L are p1 and p2.  side can have value 
// 1 or -1 specifying each of the parts made by the line L 
void findQuickHull(std::vector<iPair> points, int n, iPair p1, iPair p2, int side) 
{ 
    int ind = -1; 
    int max_dist = 0; 
  
    // finding the point with maximum distance 
    // from L and also on the specified side of L. 
    for (int i=0; i<n; i++) 
    { 
        int temp = lineDist(p1, p2, points[i]); 
        if (findSide(p1, p2, points[i]) == side && temp > max_dist) 
        { 
            ind = i; 
            max_dist = temp; 
        } 
    } 
  
    // If no point is found, add the end points 
    // of L to the convex hull. 
    if (ind == -1) 
    { 
        hull.insert(p1); 
        hull.insert(p2); 
        return; 
    } 
  
    // Recur for the two parts divided by a[ind] 
    findQuickHull(points, n, points[ind], p1, -findSide(points[ind], p1, p2)); 
    findQuickHull(points, n, points[ind], p2, -findSide(points[ind], p2, p1)); 
} 

void quickHull(std::vector<iPair> points, int n, std::string outputDataFile) 
{ 
    // a[i].second -> y-coordinate of the ith point 
    if (n < 3) 
    { 
        std::cout << "More than 3 points are needed\n"; 
        return; 
    } 
  
    // Finding the point with minimum and 
    // maximum x-coordinate 
    int min_x = 0, max_x = 0; 
    for (int i=1; i<n; i++) 
    { 
        if (points[i].first < points[min_x].first) 
            min_x = i; 
        if (points[i].first > points[max_x].first) 
            max_x = i; 
    } 
  
    // Recursively find convex hull points on 
    // one side of line joining a[min_x] and 
    // a[max_x] 
    findQuickHull(points, n, points[min_x], points[max_x], 1); 
  
    // Recursively find convex hull points on 
    // other side of line joining a[min_x] and 
    // a[max_x] 
    findQuickHull(points, n, points[min_x], points[max_x], -1); 
  
  //*****************************************

  // Hull is done, now put in order to draw it right
  // **This is a time consuming process, and can skew results on hulls with many points**
  std::set<iPair>::iterator it;
  for (it = hull.begin(); it != hull.end(); ++it) {
      iPair point = *it;
      Point myPoint;
      myPoint.x = point.first;
      myPoint.y = point.second;
      hull_vector.push_back(myPoint);
  }

  int ymin = hull_vector[0].y, min = 0;

  // Go through all points and find one with least y value
  for (int i = 1; i < hull_vector.size(); i++) {
    int y = hull_vector[i].y;

    if ((y < ymin) || (ymin == y && hull_vector[i].x < hull_vector[min].x))
      ymin = hull_vector[i].y, min = i;
  }

  // Set p0 to the lowest point we just found, then sort the rest of them
  swap(hull_vector[0], hull_vector[min]);
  p0 = hull_vector[0];

  // Sort newly converted vector of points, just like beginning of graham scan
  qsort(&hull_vector[1], hull_vector.size()-1, sizeof(Point), compare);
  
  // Export hull to file
  std::ofstream file;
  file.open(outputDataFile.c_str(), std::ofstream::out | std::ofstream::trunc);
  for (int i = 0; i < hull_vector.size(); i++) {
    file << hull_vector[i].x << "\t" << hull_vector[i].y << "\n";
  }
  file.close(); 
} 

//****END QUICK HULL****

int main(int argc, char *argv[])
{
   //Second project starts here
   if (argc < 3) 
      std::cout << "wrong format! should be \"a.exe algType dataFile\"";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];
      std::string outputFile = "";
      
      //read your data points from dataFile (see class example for the format)
      std::ifstream dataFile;
      dataFile.open(dataFilename.c_str());
      std::vector<Point> points;
      std::vector<iPair> quick_points;

    // Read datapoints from dataFile
    if (dataFile.is_open()) {
      int x, y;
      while(dataFile >> x >> y) {
        Point point;
        point.x = x;
        point.y = y;
        points.push_back(point);
        quick_points.push_back(*new iPair(x, y));
      }
    }
      
      
      if (algType[0]=='G') {
         //call your Graham Scan algorithm to solve the problem
          std::clock_t start = std::clock();
          grahamScanConvexHull(points, points.size(), "hull_G.txt");
          double duration = (std::clock() - start);
          std::cout << "Graham scan on " << dataFilename << " took " << duration <<  " milliseconds" << std::endl;
         outputFile = "hull_G.txt";
      } 
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         std::clock_t start = std::clock();
         jarvisMarchConvexHull(points, points.size(), "hull_J.txt");
         double duration = (std::clock() - start);
         std::cout << "Jarvis's March on " << dataFilename << " took " << duration <<  " milliseconds" << std::endl;
         outputFile = "hull_J.txt";
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         std::clock_t start = std::clock();
         quickHull(quick_points, points.size(), "hull_Q.txt");
         double duration = (std::clock() - start);
         std::cout << "Quickhull on " << dataFilename << " took " << duration <<  " milliseconds" << std::endl;
         outputFile = "hull_Q.txt";
      }
      
      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.
	
	return 0;
}
}