# Casting Curved Shadows on Curved Surfaces
OpenGL implementation of shadow mapping as described in the [research paper](https://github.com/krutibaraiya/CurvedShadows/blob/master/ResearchPaper.pdf) by Lance Williams, 1978.

Course Project for IS F311- Computer Graphics @ BITS Pilani, Hyderabad Campus.

<!-- ## Objective:
* To understand OpenGL Transformations, 3D viewing Pipeline and matrix stack operations.
* Incorporating objects made in 3D Modelling software such as Blender.
* Implementing camera functions such as pan ,zoom, pitch, yaw and roll to interact with the scene. -->

## Scene from observer's view:
![Scene from observer's view](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/fromobsview.png)

## Scene from light source's view:
![Scene from light source's view](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/fromlightview.png)

[Doxygen documentation](https://github.com/krutibaraiya/CurvedShadows/tree/master/html)
[Html Documentation](https://github.com/krutibaraiya/CurvedShadows/tree/master/Html%20pages/html%20pages)

## Challenges:
### 1) Shadow acne
![Shadow acne](https://github.com/krutibaraiya/CurvedShadows/blob/master/Html%20pages/images/moire1.png)

Closer look of the scene shows us distinct, alternating black lines and speckled patterns (Moire pattern). This is because of the resolution of the shadow map. When far away from the light source, multiple fragments can sample the same value from the depth map. This is an issue when the light source is inclined at an angle because the depth map is also rendered from that inclined angle. This tilted depth texel is used by multiple fragments, while some above and some are below the surface. The “raised” pixels cast small shadows onto their neighbours. Some fragments are considered to be the shadow and some aren’t, hence giving the speckled Moire pattern.

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


