# Casting Curved Shadows on Curved Surfaces
OpenGL implementation of shadow mapping as described in the [research paper](https://github.com/krutibaraiya/CurvedShadows/blob/master/ResearchPaper.pdf) by Lance Williams, 1978.

Course Project for IS F311- Computer Graphics @ BITS Pilani, Hyderabad Campus.

## Algorithm:
<!-- ![Scene from observer's view](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/fromobsview.png)

## Scene from light source's view:
![Scene from light source's view](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/fromlightview.png) -->

The algorithm described in the paper consists of two steps:
* Consider a single light source. The entire view of the scene is constructed from the light source’s point of view by computing and storing only the Z values in a depth buffer/depth map.

* Then the scene is constructed from the observer’s (camera) view by a linear transformation mapping of coordinates from observer’s view to the precomputed light source’s view. As each point is generated, it is tested for visibility i.e. if the point is not visible to the light source, then it lies in the shadow region. This is then shaded accordingly.


<table>
  <tr>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/fromobsview.png" ></td>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/fromlightview.png" ></td>
  </tr>
  <tr>
    <td>From observer's view</td>
     <td>From light source's view</td>
  </tr>
 </table>

[Doxygen documentation](https://github.com/krutibaraiya/CurvedShadows/tree/master/html)

[Html Documentation](https://github.com/krutibaraiya/CurvedShadows/tree/master/Html%20pages/html%20pages)

## Challenges:
### 1) Shadow acne
<!-- ![Shadow acne](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/moire1.png) -->

<table>
  <tr>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/moire1.png" ></td>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/moire2.png" ></td>
  </tr>
  <tr>
    <td>With shadow acne</td>
     <td>Without shadow acne</td>
  </tr>
 </table>

Closer look of the above scene on left shows us distinct, alternating black lines and speckled patterns (Moire pattern). This is because of the resolution of the shadow map. When far away from the light source, multiple fragments can sample the same value from the depth map. This is an issue when the light source is inclined at an angle because the depth map is also rendered from that inclined angle. This tilted depth texel is used by multiple fragments, while some above and some are below the surface. The “raised” pixels cast small shadows onto their neighbours. Some fragments are considered to be the shadow and some aren’t, hence giving the speckled Moire pattern.

The solution to this issue is to add a “bias” (error margin) i.e we simply offset the z-coordinates by a bias so that they aren’t incorrectly considered below the surface. After adding the bias, all samples get a depth smaller than the surface's depth and hence those “raised” pixels can’t cast a shadow anymore. Thus, removing the self shadowing/ shadow acne.

### 2) Peter Panning
<!-- ![Peter panning](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/peter1.png) -->

<table>
  <tr>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/peter1.png" ></td>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/peter2.png" ></td>
  </tr>
  <tr>
    <td>With Peter Panning</td>
     <td>Without Peter Panning</td>
  </tr>
 </table>

 After adding the bias, we notice that the objects seem detached from their shadows. It gives a hovering effect to the objects and the phenomenon is called Peter-Panning. This is because by applying bias, we are actually adding an offset to the depth. To correct this issue we keep a small, un-noticeable and varying bias which is dependent on the angle between the surface and light source. We also enable face culling with `glEnable(GL_CULL_FACE);` which solves Peter Panning for solid objects.

### 3) Aliasing
<!-- ![Aliasing](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/alias1.png) -->

<table>
  <tr>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/alias1.png" ></td>
    <td><img src="https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/alias2.png" ></td>
  </tr>
  <tr>
    <td>With jagged edges</td>
     <td>Without jagged edges</td>
  </tr>
 </table>
 In the above left image ,the edges of the shadows are extremely jagged. On the border, one pixel is white and the other is dark without any smooth transition in between them. This can be overcome by sampling the shadow map N times (16 times here) instead of one, this is PCF (Percentage Close Filtering). Use of Poisson sampling reduces aliasing. Neighbouring pixels in the small circular disc (called the Poisson disc) are sampled every time , thus creating a smooth shadow in the final output.

## Final scene:
![Final output scene](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/scene1.png)

## Camera Movements:
| Control          | Action                                            |
|------------------|---------------------------------------------------|
| W, S, A, D, ↑, ↓ | front, back, left, right, up and down respectively|
| R                | Reset camera                                      |
| Left click and drag  | Pan the scene                                 |
| Scroll mouse     | Zoom                                              |
| Q                | Quit                                              |

## How to run:
* The project is tested on Linux and built in C++ using the following libraries:
    * `GLFW`
    * `GLEW`
    * `GLM`
    * `ASSIMP`
    * `CMake`
* To build : `cmake -G "CodeBlocks - Unix Makefiles" /path/to/CurvedShadows`
* Run the output executable : `./CurvedShadows`

## Team members:
* [Sristi Sharma](https://github.com/judyhopps24)
* [Kruti Baraiya](https://github.com/krutibaraiya)
* [Jay Patel](https://github.com/pjay20301)


