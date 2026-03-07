#include <iostream>
#include <fstream>
#include <string>

// Please also try document your thought process and your code as you go!

// research:
// voronoi diagram
// - some way of mapping coordinates to the closest point given a set of points
// - can be used to create a delaunay triangulation as its simply formed by
// joining points of neighbouring voronoi cells

// delaunay triangulation
// - some way of producing a mesh structure / nicely formed triangles given
// a set of points
// - property: circumference of all circle containing 3 vertices in a triangle
// does not container any other vertice in inside itself

// spline
// - some approximation of a smooth cubic curve through set of some points
// - solved using linear algrebra
// - more application towards after path has been found??
// - set points that car must past through and smooth it so car isn't make 90
// degree turns

// algorithm ideas:
// could be quite easy if we can abstract the cones into some set of midpoints?
// can then be solved like a regular graph problem?
//
// idea 1:
// - can use just voronoi diagrams? at any point we know the closest cone
// - move towards the opposite direction and repeat everytime we move into a new
// cell?
// - need some kind of guiding instinct for "opposite" direction
//
// idea 2:
// - create a delaunay triangulation and the path becomes the one that connects
// the midpoint of all triangles
// - to move from point p to point p', p' must exist within the triangle that
// neighbours the triangle containing p
// - which property of delaunay triangulation can guarantee that the car will
// never go out of bounds?
//
// off-topic spline:
// - spline seems to be quite clearly focused on producing a smoother path since
// both above ideas can lead to a jagged path
// - probably ignore this for now

// implementation:
// - going with idea 2, how the heck will i ever do this?
// - online library in c++: CGAL

int main() {
    return 0;
}