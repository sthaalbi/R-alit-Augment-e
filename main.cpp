// Std. Includes
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AprilTagReader.h"
#include "apriltag.h"

// Callback de fermeture de la fenêtre
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    /************ Initialisation Fenetre, Shaders, texture ***********/
    
    GLuint screenWidth = 800, screenHeight = 600;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

   
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Define the viewport dimensions
    int width, height;
    // On recupere les dimensions de la fenetre creee plus haut
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glEnable(GL_DEPTH_TEST);

    Shader shader("opengl_code/shaders/default.vertexshader", "opengl_code/shaders/default.fragmentshader");
    Shader lightshader("opengl_code/shaders/light.vertexshader", "opengl_code/shaders/light.fragmentshader");

    GLuint texture; // Declaration de l'identifiant

	glGenTextures(1, &texture); // Generation de la texture
	// On bind la texture cree dans le contexte global d'OpenGL
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Modification des parametres de la texture
	// Methode de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	// Methode de filtrage
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // On unbind la texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
    
    /*********************************************************************/
    
    /****************** Initialisation de l'image 2D *********************/

	GLfloat vertices[] = {
        /*     Positions    |      Normales     |     UV     */
       1.0f,  0.75f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top Right
       1.0f, -0.75f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
      -1.0f, -0.75f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
      -1.0f,  0.75f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f  // Top Left
    };
    
    GLshort indices[]{
    	0, 1, 3,
    	1, 2, 3
    };    
    
    GLuint VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    // On met notre EBO dans le contexte global
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // On assigne au EBO le tableau d'indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribut des positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Attribut des normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Attribut des UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    /*********************************************************************/

    /******************* Chargement des modeles 3D ***********************/

    /*!|| A REMPLIR ||*/
    Model suzanneModel("./opengl_code/model/suzanne/suzanne.obj");
    /*********************************************************************/
    
    /******************* Initialisation du detecteur *********************/

    // Instanciation
    /*!|| A REMPLIR ||*/
    //float f = 0; // Focal de votre caméra
    //AprilTagReader reader(f);

    AprilTagReader reader (735.115, 0, 640, 480 ,320, 240, 16.5/2);
//(float focal, int camera_id, int width, int height, float cx, float cy, float tagSize)
    
    /*********************************************************************/
    
    /*************** Création de la caméra virtuelle *********************/

    
    // Matrice de projection
    /*!|| A REMPLIR ||*/
    glm::mat4 projection = glm::perspective(
        GLfloat(glm::radians(47.0476)), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );


    // Matrice de position de la caméra
    //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -projection[0][0]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    // Envoie des matrices de projection et de vue aux shaders
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    lightshader.Use();
    glUniformMatrix4fv(glGetUniformLocation(lightshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(lightshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    /*********************************************************************/

    cv::Mat image = reader.getImage();

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Récupération des evenements et maj du fond
        glfwPollEvents();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        
        /******************* MaJ de la texture (image) ***********************/

        shader.Use();
        // Récupération de la 1ere image
        image = reader.getImage();

        //reader.getTags();

        glActiveTexture(GL_TEXTURE0 );//  Binding  de  notre  texture
		glBindTexture(GL_TEXTURE_2D , texture );//  Association du numero de la texture  pour le  shader
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows,0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
		glUniform1i(glGetUniformLocation(shader.Program , "modelTexture"), 0);        
        glm::mat4 texture_model(1.0f); // Matrice identite
        // On passe la position de l'objet au shader
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(texture_model));



        /*********************************************************************/

        /************************ On dessine l'image *************************/

        glBindVertexArray(VAO);
        // On dessine l'objet courant 
        glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        /*********************************************************************/
        glClear(GL_DEPTH_BUFFER_BIT);
        /********************* Affichage des éléments 3D *********************/
        glm::mat4 model(1.0f);
        lightshader.Use();

        /*!|| A REMPLIR ||*/
        std::vector<TagData> tags = reader.getTags();

        for(int i=0;i<tags.size();i++){
            model[3][0] = tags[i].position[0];
            model[3][1] = tags[i].position[1];
            model[3][2] = tags[i].position[2];
        }

        glm::mat4 scale = glm::scale(model, glm::vec3(2.0f, 2.0f ,2.0f));
        glm::mat4 rotate = glm::rotate(scale, glm::radians(180.0f),glm::vec3(1.0f, 0.0f ,0.0f));
        //glm::mat4 mvp =  scale * rotate * model;
        glUniformMatrix4fv(glGetUniformLocation(lightshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(rotate));

        //int myId = reader.idTag->id;
        //std::cout<<myId<<std::endl;
        if (tags.size()>0)
        {
            //hiba

            for(int j = 0 ; j<tags.size(); j++)
            {
                std<<cout<<tags[j].tagId<<std::endl;
            }

                suzanneModel.Draw(lightshader);
        }

        //glm::mat4 mvp = projection * view * model;
        /*********************************************************************/
    
        // Mise à jour de l'affichage
        glfwSwapBuffers(window);
    }
    
    // Libération de la mémoire
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}

// Callback de fermeture de la fenêtre
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
