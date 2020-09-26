#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const double PERCENTAGE = 0.6;

template < class T >
class Point {
 public:
  T x,y,z;
  Point (T a, T b, T c) : x(a), y(b), z(c) {}   
  Point () : x(-1), y(-1), z(-1) {}  
  bool operator == (Point < T > p){
    return this->x == p.x and this->y == p.y and this->z == p.z;
  }
  void print(){
    cout << "(" << x << "," << y << "," << z << ")\n";
  }   
};

template < class T >
class Node{
 public:
  long long numberPoints, numberCurrentPoints;    
  bool state;    
  Node < T > * children[8];        
  Point < T > limit1, limit2;
  vector < Point < T > > points;        
  Node ( Point < T > a, Point < T > b) {
    numberCurrentPoints = 0;
    state = 1; 
    limit1 = a; 
    limit2 = b;
    numberPoints = (limit2.x - limit1.x) * (limit2.y - limit1.y) * (limit2.z - limit1.z) * PERCENTAGE;
    numberPoints = (numberPoints < 1) ? 1 : numberPoints;                      
    for (int i = 0; i < 8; ++i){
      children[i] = 0;
    }
  }
};

template < class T >
class Octree{
 public:
  Node < T > * root;
  Octree (Point < T > a, Point < T > b){
    root= new Node < T > (a, b) ;                        
  }
  void insert (Point < T > p, Node < T > * &ptr, bool ok);  
  bool find (Point < T > p, Node < T > * &ptr);  
  void getMiddle (Node < T > * ptr, T &xMid, T &yMid, T &zMid);
  bool outOfLimits (Point < T > p, Node < T > * ptr); 
  int getPos (Point < T > p, T xMid, T yMid, T zMid); 
  void createNode (Node < T > * ptr, int pos, T xMid, T yMid, T zMid);
  void erase (Point < T > p, Node < T > * & ptr);  
};

template < class T >
void Octree < T > :: getMiddle (Node < T > * ptr, T &xMid, T &yMid, T &zMid){
  xMid = ( ptr->limit1.x + ptr->limit2.x ) / 2;
  yMid = ( ptr->limit1.y + ptr->limit2.y ) / 2;
  zMid = ( ptr->limit1.z + ptr->limit2.z ) / 2;    
}

template < class T >
bool Octree < T > :: outOfLimits(Point < T > p, Node < T > * ptr){
  if (p.x < ptr->limit1.x or p.x > ptr->limit2.x or
      p.y < ptr->limit1.y or p.y > ptr->limit2.y or
      p.z < ptr->limit1.z or p.z > ptr->limit2.z) {
      return 1;
  }
  return 0;        
}

template < class T >
int Octree < T > :: getPos (Point < T > p, T xMid, T yMid, T zMid){
  if(p.x <= xMid and p.y <= yMid and p.z <= zMid) { return 0; }
  if(p.x > xMid and p.y <= yMid and p.z <= zMid) { return 1; }
  if(p.x <= xMid and p.y > yMid and p.z <= zMid) { return 2; }
  if(p.x > xMid and p.y > yMid and p.z <= zMid) { return 3; }
  if(p.x <= xMid and p.y <= yMid and p.z > zMid) { return 4; }
  if(p.x > xMid and p.y <= yMid and p.z > zMid) { return 5; }
  if(p.x <= xMid and p.y > yMid and p.z > zMid) { return 6; }
  if(p.x > xMid and p.y > yMid and p.z > zMid) { return 7; }
}

template < class T >
void Octree < T > :: createNode (Node < T > * ptr, int pos, T xMid, T yMid, T zMid){
  Point < T > a, b;
  if (pos == 0) {
    a.x = ptr->limit1.x; a.y = ptr->limit1.y; a.z = ptr->limit1.z;
    b.x = xMid; b.y = yMid; b.z = zMid;       
  } else if (pos == 1) {
    a.x = xMid; a.y = ptr->limit1.y; a.z = ptr->limit1.z;
    b.x = ptr->limit2.x; b.y = yMid; b.z = zMid;
  } else if (pos == 2) {
    a.x = ptr->limit1.x; a.y = yMid; a.z = ptr->limit1.z;
    b.x = xMid; b.y = ptr->limit2.y; b.z = zMid;
  } else if (pos == 3) {
    a.x = xMid; a.y = yMid; a.z = ptr->limit1.z;
    b.x = ptr->limit2.x; b.y = ptr->limit2.y; b.z = zMid;
  } else if (pos == 4) {
    a.x = ptr->limit1.x; a.y = ptr->limit1.y; a.z= zMid;
    b.x = xMid; b.y = yMid; b.z = ptr->limit2.z;
  } else if (pos == 5) {
    a.x = xMid; a.y = ptr->limit1.y; a.z = zMid;
    b.x = ptr->limit2.x; b.y = yMid; b.z = ptr->limit2.z;
  } else if (pos == 6) {
    a.x = ptr->limit1.x; a.y = yMid; a.z = zMid;
    b.x = xMid; b.y = ptr->limit2.y; b.z = ptr->limit2.z;
  } else if (pos == 7) {
    a.x = xMid; a.y = yMid; a.z = zMid;
    b.x = ptr->limit2.x; b.y = ptr->limit2.y; b.z = ptr->limit2.z;
  }
  ptr->children[pos] = new Node < T > (a, b);
}

template < class T >
bool Octree < T > :: find (Point < T > p, Node < T > * &ptr){
  if (outOfLimits(p,ptr)) return 0;
  if (ptr->numberCurrentPoints >= ptr->numberPoints) { 
    for (int i = 0; i < ptr->points.size(); ++i) {
      if (ptr->points[i] == p) return 1;
    }
    return 0;
  } else {
    T xMid, yMid, zMid;
    getMiddle (ptr, xMid, yMid, zMid);
    int pos = getPos (p, xMid, yMid, zMid);
    if (ptr->children[pos] == 0 or ptr->children[pos]->state == 0) return 0;
    find (p, ptr->children[pos]);       
  }    
}

template < class T >
void Octree < T > :: insert(Point < T > p, Node < T > * & ptr, bool ok){
  if (find (p, ptr)) return;            
  if (outOfLimits (p, ptr)) return;
  if (ptr->numberCurrentPoints+1 >= ptr->numberPoints) {
    vector < Point < T > > tmp;
    for(int i = 0; i < 8; ++i) {
      if (ptr->children[i] == 0) continue;
      for (int j = 0; j < ptr->children[i]->points.size(); j++) {
        tmp.emplace_back(ptr->children[i]->points[j]);
      }
      ptr->children[i] = 0;
    }
    for (int i = 0; i < ptr->points.size(); i++) {
      tmp.emplace_back(ptr->points[i]);
    }
    tmp.emplace_back(p);
    ptr->points = tmp;
    ptr->numberCurrentPoints++;
     
  } else {
    if (ok){
      ptr->numberCurrentPoints++;
    }
    T xMid, yMid, zMid;
    getMiddle (ptr, xMid, yMid, zMid);
    int pos = getPos (p, xMid, yMid, zMid);
    if (ptr->children[pos] == 0 or ptr->children[pos]->state == 0) {
      createNode(ptr, pos , xMid, yMid, zMid);                        
    }
    insert (p, ptr->children[pos], ok);        
  }    
}          

template < class T >
void Octree < T > :: erase(Point < T > p, Node < T > * & ptr){
  if (!find (p,ptr)) return;
  if (outOfLimits (p,ptr)) return;
  if (ptr->numberCurrentPoints >= ptr->numberPoints) {
    vector < Point < T > > tmp; 
    for (int i = 0; i < ptr->points.size(); ++i) {
      if (ptr->points[i] == p) continue;
      tmp.emplace_back(ptr->points[i]);      
    }
    ptr->numberCurrentPoints = 0;
    ptr->points.clear();
    if (tmp.size() == 0){
      ptr->state = 0;
    }else{
      for(int i = 0; i < tmp.size(); ++i){
        insert(tmp[i], root, 0);
      }
    }
  } else {
    T xMid, yMid, zMid;
    getMiddle (ptr, xMid, yMid, zMid);
    int pos = getPos (p, xMid, yMid, zMid);
    ptr->numberCurrentPoints--;
    if (ptr->children[pos] == 0 or ptr->children[pos]->state == 0) return;
    erase (p, ptr->children[pos]);       
  }
}  

// This function returns a vector with the points read from the "points.obj" file 
vector < vector < int > > readPoints(){
  ifstream input ("../files/points.obj");
  vector < vector < int > > points;
  string s;
  float x, y, z;
  while (!input.eof()){
    input >> s;
    if(s != "v") break;
    input >> x >> y >> z;
    x *= 100; y *= 100; z *= 100; // This allows working with points of integer type
    x = int(x); y = int(y); z = int(z);
    vector < int > tmp {x, y, z};
    points.emplace_back(tmp);
  }
  return points;
}

template <class T>
void insertPoints (Octree < T > & octree, vector < vector < T > > points){
  for (int i = 0; i < points.size(); ++i) {
    Point < T > p (points[i][0], points[i][1], points[i][2]);
    Node < T > * ptr = octree.root;
    octree.insert(p, ptr, 1);
  }    
}

// This function checks if all points have been inserted to the Octree
void check (Octree < int > octree, vector < vector < int > > points){     
  for (int i = 0; i < points.size(); ++i) {
    Node < int> * ptr = octree.root;
    Point < int > p (points[i][0], points[i][1], points[i][2]);
    bool ok = octree.find(p, ptr);
    if (!ok) {
      cout << p.x << " " << p.y << " " << p.z << "\n";
      cout << "Error.\n";
      return;
    }
  }
  cout<<"That's all right.\n";
}

// This function erases all the points of the Octree
void eraseAll (Octree < int > octree, vector < vector < int > > points){     
  for (int i = 0; i < points.size(); ++i) {
    Node < int> * ptr = octree.root;
    Point < int > p (points[i][0], points[i][1], points[i][2]);
    octree.erase(p,ptr);
  }
}

int main(){
  Point < int > A (-500, -300, -100), B (400, 600, 800);  // lower and upper bound of the initial space
  Octree < int > octree (A, B);
  vector < vector < int > > points = readPoints ();
  insertPoints <int> (octree, points);
  
  // This are test points
  Point < int > a (248, -153, 514);
  Point < int > b (-308, -58, 36);
  Node < int > * p1 = octree.root;
  Node < int > * p2 = octree.root;
  cout << octree.find (a, p1) << "\n";
  cout << octree.find (b, p2) << "\n";
  octree.erase (a, p1);
  octree.erase (b, p2);
  cout << octree.find (a, p1) << "\n";
  cout << octree.find (b, p2) << "\n";
  
  return 0;
}