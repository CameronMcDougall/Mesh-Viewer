#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "opengl.h"
#include "imgui.h"

#include "cgra/matrix.hpp"
#include "cgra/wavefront.hpp"

#include "ex1.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

void Application::init() {
    // Load the shader program
    // The use of CGRA_SRCDIR "/path/to/shader" is so you don't
    // have to run the program from a specific folder.
    m_program = cgra::Program::load_program(
        CGRA_SRCDIR "/res/shaders/simple.vs.glsl",
        CGRA_SRCDIR "/res/shaders/simple.fs.glsl");

    // Create a view matrix that positions the camera
    // 10 units behind the object
    glm::mat4 viewMatrix(1);
    viewMatrix[3] = glm::vec4(0, 0, -10, 1);
    m_program.setViewMatrix(viewMatrix);

    // Create the cube mesh
   createCube();
}

void Application::createCube() {
    /************************************************************
     * 2. Create a Mesh                                         *
     *                                                          *
     * Add the remaining triangles for the cube                 *
     ************************************************************/

    // Use the correct number of rows for the full
    // cube.
    cgra::Matrix<double> vertices(8, 3);
    cgra::Matrix<unsigned int> triangles(12, 3);
	vertices.setRow(0, {-1,-1,1});
	vertices.setRow(1, { 1,-1,1 });
	vertices.setRow(2, { 1,1,1 });
	vertices.setRow(3, { -1,1,1 });
	vertices.setRow(4, { -1,-1,-1 });
	vertices.setRow(5, { 1,-1,-1 });
	vertices.setRow(6, { 1,1,-1});
	vertices.setRow(7, { -1,1,-1 });
	//front
	triangles.setRow(0, { 0,1,2});
	triangles.setRow(1, { 2,3,0 });
	//right
	triangles.setRow(2, { 1,5,6 });
	triangles.setRow(3, { 6,2,1 });
	//back
	triangles.setRow(4, { 5,4,7 });
	triangles.setRow(5, { 7,6,5 });
	//left
	triangles.setRow(6, { 4,0,3 });
	triangles.setRow(7, { 3,7,4 });
	//top
	triangles.setRow(8, { 3,2,6 });
	triangles.setRow(9, { 6,7,3 });
	//bottom
	triangles.setRow(10, { 4,5,0 });
	triangles.setRow(11, { 1,0,5 });
    // Remember to make sure that the order of the vertices
    // is counter-clockwise when looking at the front of the
    // triangle.
	
    m_mesh.setData(vertices, triangles);
}

void Application::loadObj(const char *filename) {
    cgra::Wavefront obj;
    // Wrap the loading in a try..catch block
    try {
         obj = cgra::Wavefront::load(filename);
    } catch (std::exception e) {
        std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
        return;
    }

    /************************************************************
     * 2. Create a Mesh                                         *
     *                                                          *
     * Use the data in `obj` to create appropriately-sized      *
     * vertex and index matrices and fill them with data.       *
     *                                                          *
     * Assume that all the faces in the file are triangles.     *
     ************************************************************/
    // Replace these with appropriate values
    unsigned int numVertices  = obj.m_positions.size();
    unsigned int numTriangles = obj.m_faces.size();
    cgra::Matrix<double> vertices(numVertices, 3);
    cgra::Matrix<unsigned int> triangles(numTriangles, 3);
    for (size_t i = 0; i < obj.m_positions.size(); i++) {
        // Add each position to the vertices matrix
		glm::vec4 pos = obj.m_positions.at(i);
		vertices.setRow(i, { pos.x, pos.y, pos.z});
    }
    for (size_t i = 0; i < obj.m_faces.size(); i++) {
        // Add each triangle's indices to the triangles matrix
        // Remember that Wavefront files use indices that start at 1
		cgra::Wavefront::Face pos= obj.m_faces.at(i);
		std::vector<cgra::Wavefront::Vertex> a = pos.m_vertices;
		triangles.setRow(i, {a.at(0).m_p-1, a.at(1).m_p-1, a.at(2).m_p-1});
    }
    m_mesh.setData(vertices, triangles);
}
bool q = false;
void Application::drawScene() {
    // Calculate the aspect ratio of the viewport;
    // width / height
    float aspectRatio = m_viewportSize.x / m_viewportSize.y;
    // Calculate the projection matrix with a field-of-view of 45 degrees
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // Set the projection matrix
    m_program.setProjectionMatrix(projectionMatrix);

    glm::mat4 modelTransform(1.0f);

    /************************************************************
     * 3. Manual Transforms                                     *
     *                                                          *
     * Use `m_translation`, `m_scale`, and `m_rotationMatrix`   *
     * to create the  `modelTransform` matrix.                  *
     * The following glm functions will be useful:              *
     *    `glm::translate`                                      *
     *    `glm::scale`                                          *
     ************************************************************/
	modelTransform = glm::scale(modelTransform, glm::vec3(m_scale, m_scale, m_scale));
	modelTransform = m_rotationMatrix* modelTransform;
	modelTransform = glm::translate(modelTransform, m_translation);
	m_program.setModelMatrix(modelTransform);
    // Draw the mesh
    m_mesh.draw();
}
char filePath[255];
void Application::doGUI() {
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Shapes");
	//cube
	if (ImGui::Button("Cube", ImVec2(70, 20))) {	
		createCube();
	}
	ImGui::InputText("FilePath", filePath, 255);
	const char* finalWord = filePath;
	if (ImGui::Button("Load .obj", ImVec2(70, 20))) {
		loadObj(finalWord);
	}
    /************************************************************
     *                                                          *
     * 2. Load an .obj file                                     *
     *                                                          *
     * Add an input for a filename.                             *
     *                                                          *
     ************************************************************
     *                                                          *
     * 3. Manual Transforms                                     *
     *                                                          *
     * Create inputs for controlling translation, scale and     *
     * rotation.                                                *
     *                                                          *
     ************************************************************
     *                                                          *
     * 5. Add a checkbox for rendering the object in wireframe  *
     *  mode.                                                   *
     ************************************************************/
	 static bool wire = false;
	if (ImGui::Checkbox("Wireframe",&wire) ) {
		m_mesh.setDrawWireframe(!m_mesh.getDrawWireframe());
	}
    // Example for rotation, use glm to create a a rotation
    // matrix from this vector
    static glm::vec3 rotation(0, 0, 0);
	static float scale = 1.0;
    if (ImGui::InputFloat3("Rotation", &rotation[0])) {
        // This block is executed if the input changes
		m_rotationMatrix= glm::eulerAngleXYZ(rotation[0], rotation[1], rotation[2]);
    }
	if (ImGui::InputFloat3("Translate", &m_translation[0]));
	if (ImGui::InputFloat("Scale", &m_scale));
		// This block is exe9cuted if the input changes
    ImGui::End();
}


// Input Handlers

void Application::onMouseButton(int button, int action, int) {
    if (button >=0 && button < 3) {
        // Set the 'down' state for the appropriate mouse button
        m_mouseButtonDown[button] = action == GLFW_PRESS;
    }
}

void Application::onCursorPos(double xpos, double ypos) {
    /**********************************************************
     * 4. Interactive Transforms                              *
     *                                                        *
     * Change `translation`, `scale` and `rotation` based on  *
     * `mousePositionDelta`.                                  *
     **********************************************************/

    // Make a vec2 with the current mouse position
    glm::vec2 currentMousePosition(xpos, ypos);
    // Get the difference from the previous mouse position
    glm::vec2 mousePositionDelta = (currentMousePosition - m_mousePosition);
    if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_LEFT]) {	
		glm::vec2 s1 = m_mousePosition;
		glm::vec2 s2 = currentMousePosition;
		glm::vec2 center = glm::vec2((m_viewportSize.x)/2 , (m_viewportSize.y)/2 );
		s1 -= center;
		s2 -= center;
		s1 /= glm::length(center);
		s2/= glm::length(center);
		glm::vec3 v0(s1.x,-s1.y,0);
		glm::vec3 v1(s2.x, -s2.y, 0);
		float v0Sq = glm::length(v0);
		float v1Sq = glm::length(v1);
		v0.z = sqrt(1 - (v0Sq* v0Sq));
		v1.z = sqrt(1 - (v1Sq* v1Sq));
		float dotProduct = glm::dot(v0, v1);
		float angle = glm::acos(dotProduct);
		glm::mat4 model(1.0f);
		glm::vec3 cross = glm::cross(v0, v1);
		if (glm::length(cross) > 0) {
			model = glm::rotate(model, angle, cross)*model;
			m_rotationMatrix = model*m_rotationMatrix;
		}
	} else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {
		m_translation += glm::vec3(-mousePositionDelta.x / 10, mousePositionDelta.y / 10, 0);
	}
	else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {
		m_scale += ((mousePositionDelta.x + mousePositionDelta.y) / 2) / 10;
	}
    // Update the mouse position to the current one
    m_mousePosition = currentMousePosition;
}

void Application::onKey(int key, int scancode, int action, int mods) {
    // `(void)foo` suppresses unused variable warnings
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

void Application::onScroll(double xoffset, double yoffset) {
    // `(void)foo` suppresses unused variable warnings
    (void)xoffset;
    (void)yoffset;
}
