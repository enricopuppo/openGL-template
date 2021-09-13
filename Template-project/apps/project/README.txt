--> CGAR Final Project (Computer Graphics part) <--

Group members: Andrea Gatti (4496922), Matteo Martini (3922943)

Project specification (custom):
	- C++ framework with OpenGL, GLFW, GLEW, GLM and ImGui
	- Scene with a swimming pool composed by various objects, imported and/or generated
	- First and third person view with with toggle
	- Third person view managed with quaternions
	- Basic effects on water
	- Controllable submarine inside the pool
	- Illumination and textures

Extras added:
	- OBJ file parser
	- Reflection and refraction on water with projective texture mapping, clip planes 
	  and custom frame buffers, plus Fresnel effect and normals for waves
	- Multi-light setup with customizable color and attenuation
	- Extra procedural shapes in addition to the ones used in the labs: quad, pool with
	  duplicated vertices, sphere clove and whole sphere
	- Texture blend map for terrain texturing
	- Predisposed terrain model for height maps (not used for time constraints)
	- Alpha channel management for textures and mipmapping
	- Day/night shift with constant lights
	- Optimized and more efficient per-model rendering procedure
	
Install and run:
	GLM and ImGui are included in the program, so it is necessary to have a working
	version of OpenGL, GLFW and GLEW installed on the machine.
	If so it should be sufficient to call 'make' in order to build the program.
	'make clean' removes all the semi-finished files.
	The controls for the simulation will appear in a dedicated collapsible sub-window when the program is running.
