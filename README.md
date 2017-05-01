# Game Physics Cookbook

This book is a comprehensive guide to the linear algebra and collision detection games commonly use, but only briefly touches on the topic of collision resolution (Physics). The book is structured as follows:

* __Chapters 1, 2 and 3__    cover the basics Linear Algebra.
* __Chapters 4, 5 and 6__    cover two dimensional primitives and how to detect intersections between them. 
* __Chapters 7, 8, 9 and 10__   cover three dimensional primitives and the most efficient way to determine intersections between them. 
* __Chapters 11, 12 and 13__ cover meshes, scenes and scene organization.
* __Chapters 14, 15 and 16__ cover physics. Throughout these three chapters we built a very basic rigid body physics engine.

All of of the topics covered in this book are used to progressivley build a rigid body physics engine. The final three chapters (14, 15 and 16) implement particle physics, rigid body physics and soft body physics (cloth). An appendinx is provided which briefly covers advanced topics, resources for exploring these topics as well as additional resources for exploring game physics.

## Figures

Some of the figures created for the book did not translate well to print. To address this I have published all of the figures used in the book online at: [https://github.com/gamephysicscookbook/Figures](https://github.com/gamephysicscookbook/Figures)

## Math

The first three chapters of the book are dedicated to teaching the basic linear algebra needed for game development. Every concept is explained in a mathematical context, source code is provided for every concept and images are also provided when something can be explored visually. Troughout the first three chapters the following data structures are created:

* 2 dimensional vector (```vec2```)
* 3 dimensional vector (```vec3```)
* 2x2 matrix (```mat2```)
* 3x3 matrix (```mat3```)
* 4x4 matrix (```mat4```)

When possible, matrix operations are implemented in a generic way. For example, code is provided to multiply two matrices of arbitrary sizes together.

## Collision Detection

The following intersections are covered in the book:

|   | Point | Line | Ray | Sphere | AABB | OBB | Plane | Triangle| Frustum |
|:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| __Point__ | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![no](img/no.png) |
| __Line__ | ![yes](img/yes.png) | ![no](img/no.png) | ![no](img/no.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![no](img/no.png) |
| __Ray__ | ![yes](img/yes.png) | ![no](img/no.png) | ![no](img/no.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![no](img/no.png) |
| __Sphere__ | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) |
| __AABB__ | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) |
| __OBB__ | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) |
| __Plane__ | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![no](img/no.png) |
| __Triangle__ | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![no](img/no.png) |
| __Frustum__ | ![no](img/no.png) | ![no](img/no.png) | ![no](img/no.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![yes](img/yes.png) | ![no](img/no.png) | ![no](img/no.png) | ![no](img/no.png) |

## Physics

Chapter 14 covers naive particle physics, this chapter is meant as an introduction to setting up a physics loop and considering the general format of a physics loop. Chapter 15 is the most interesting chapter, it implements a basic rigid body physics engine. The basic engine has support for oriented boxes and spheres, stacking can be made to work but is not directly supported. Chapter 16 covers springs and how springs can be used to implement soft body physics. The final demo of the book is a soft body, cloth physics demo.

![Chapter 14 demo GIF](img/chapter14.gif)

![Chapter 15 demo GIF](img/chapter15.gif)

![Chapter 16 demo GIF](img/chapter16.gif)

## Learnings

This being my first book, I've learned a lot about the writing process; perhaps even more about planning. The following is a list of things I've discovered while writing this book that I did not plan properly for:

* __Quaternions__:
  Having a quaternion is a must! I planned to write the book using euler rotations and rotation matrices. While this worked, having access to quaternions would have made life a lot easyer.

* __Planes__:
  Plane intersections should not return a boolean value, they should classify the intersection as: behind, intersecting, in front. Preferably, if the intersection is in front or behind, you want to return some indication of distance. This becomes very useful when doing Frustum culling. I didn't know how in-depth i was going to write about frustums, so i decided to do simple boolean intersectiosn.

* __Raycasting__:
  I should have written raycasts to return a raycast result from the beginning. Having to later go back and re-write the raycasting API turned out to be more complicated to express in text than i anticipated. The tought behind this was to keep things simple at first, complicate only as needed.

## Issues

There are various issues in the physics implementation of the source code. These issues come from the fact that the physics part of this book had to be condensed into three chapters. There was just not enough time to cover everything needed to make a robust rigid-body physics system. The biggest problem with the engine is the fact that __there is no Arbiter__. 

Without an arbiter, we can't build a sequential impulse solver. This leaves us with a rather bare-bones naive impulse solver. The problem is, impulses are solved per contact per frame. This causes excessive sliding. I've compensated for the lack of sequential impulse with linear projection and agressive friciton biasing. Older physics engines do something similar, by relying on strong handed sleeping to mask the issue. Needless to say, sleeping was not implemented.

## Future

If I have a chance to write a second edition of this book, i will remove the chapters on two dimensional collisions (chapters 4, 5 and 6) and cut down the page count of the scene management chapters. I plan to use the extra pages to cover the following topics:

* In depth discussion and implementation of [GJK](https://www.youtube.com/watch?v=Qupqu1xe7Io)
* A proper arbiter system and a more stable collision resolution model.
* Capsule primitive
* Convex hull generation and collision testing

## Book Info

* ISBN-13: 9781787123663
* [View at Packt](https://www.packtpub.com/game-development/game-physics-cookbook)
* [View on Amazon](https://www.amazon.com/Game-Physics-Cookbook-Gabor-Szauer/dp/1787123669/)
* [All my publications](http://gaborszauer.com/publication.html)

## Table Of Contents

1. Vectors
   - Introduction
   - Vector definition
   - Component wise operations
   - Dot product
   - Magnitude
   - Normalizing
   - Cross product
   - Angles
   - Projection
   - Reflection
2. Matrices
   - Introduction
   - Matrix definition
   - Transpose
   - Multiplication
   - Identity matrix
   - Determinant of a 2x2 matrix
   - Matrix of minors
   - Cofactor
   - Determinant of a 3x3 matrix
   - Operations on a 4x4 matrix
   - Adjugate matrix
   - Matrix inverse
3. Matrix Transforms
   - Introduction
   - Matrix majors
   - Translation
   - Scaling
   - How rotations work
   - Rotation matrices
   - Axis angle rotation
   - Vector matrix multiplication
   - Transform matrix
   - Projection matrix
4. 2D Primitive Shapes
   - Introduction
   - 2D points
   - 2D lines
   - Circle
   - Rectangle
   - Oriented rectangle
   - Point containment
   - Line intersection
5. 2D Collisions
   - Introduction
   - Circle to circle
   - Circle to rectangle
   - Circle to oriented rectangle
   - Rectangle to rectangle
   - Seperating Axis Theorem
   - Rectangle to oriented rectangle
   - Oriented rectangle to oriented rectangle
6. 2D Optimizations
   - Introduction
   - Containing circle
   - Containing rectangle
   - Simple and complex shapes
   - Quad tree
   - Broad phase collisions
7. 3D Primitive Shapes
   - Introduction
   - Point
   - Line segment
   - Ray
   - Sphere
   - Axis aligned bounding box
   - Oriented buonding box
   - Plane
   - Triangle
8. 3D Point Tests
   - Introduction
   - Point and sphere
   - Point and axis aligned bounding box
   - Point and oriented bounding box
   - Point and plane
   - Point and line
   - Point and ray
9. 3D Shape Intersections
   - Introduction
   - Sphere to sphere
   - Sphere to axis aligned bounding box
   - Sphere to oriented bounding box
   - Sphere to plane
   - Axis aligned bounding box to axis aligned bounding box
   - Axis aligned bounding box to oriented bounding box
   - Axis aligned bounding box to plane
   - Oriented bounding box to oriented bounding box
   - Oriented bounding box to plane
   - Plane to plane
1. 3D Line Intersections
   - Introduction
   - Raycast sphere
   - Raycast axis aligned bounding box
   - Raycast oriented bounding box
   - Raycast plane
   - Linetest sphere
   - Linetest axis aligned bounding box
   - Linetest oriented bounding box
   - Linetest plane
1. Triangles and Meshes
   - Introduction
   - Point in triangle
   - Closest point triangle
   - Triangle to sphere
   - Triangle to axis aligned bounding box
   - Triangle to oriented box
   - Triangle to plane
   - Triangle to triangle
   - robustness of the seperating axis theorem
   - Raycast triangle
   - Linetest triangle
   - Mesh object
   - Mesh optimization
   - Mesh operations
2. Models and Scenes
   - Introduction
   - The model object
   - Operations on models
   - The scene object
   - Operations on the scene
   - The octree object
   - Operations on the octree
   - Octree and scene integration
3. Camera and Frustum
   - Introduction
   - Camera object
   - Camera controls
   - Frustum object
   - Frusum from matrix
   - Sphere in frustum
   - Bounding box in frustum
   - Octree culling
   - Picking
4. Constraint Solving
   - Introduction
   - Framework introduction
   - Raycast sphere
   - Raycast bounding box
   - Raycast plane and triangle
   - Physics system
   - Integrating particles
   - Solving constraints
   - Verlet integration
5. Manifolds and Impulses
   - Introduction
   - Manifold for spheres
   - Manifold for boxes
   - Rigid body modifications
   - Linera velocity
   - Linear impulse
   - Physics system update
   - Angular velocity
   - Angular impulse
6. Sprints and Joints
   - Introductions
   - Particle modifications
   - Springs
   - Cloth
   - Physics System modification
   - Joints
7. Appendix: Advanced Topics
   - Introduction
   - Generic collisions
   - Stability improvements
   - Sprints
   - Open source physics engines
   - Books
   - Online resources
   - Summary
   
## Cover

![Cover Image](img/cover.png)
