#pragma once

// Std. Includes
#include <vector>

// GL Includes

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QtGui/QOpenGLFunctions_4_5_Compatibility>
#include <QtCore/QDebug>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW = 0.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat SSENSITIVTY = 0.25f/60.0f;
const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera:protected QOpenGLFunctions_4_5_Compatibility
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 InitFront;
    glm::vec3 InitUp;
    glm::vec3 Right;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat ScrollSensitivity;
    GLfloat Zoom;
    GLfloat Size;
    GLfloat PositionOffset;

    //// Constructor with vectors
    //Camera(GLfloat size ,glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
    //    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 front = glm::vec3(-1.0f, 0.0f, 0.0f),
    //    GLfloat yaw = YAW, GLfloat pitch = PITCH, GLfloat position_offset = 0) 
    //    : MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    //{
    //    up = glm::normalize(up); front = glm::normalize(front);
    //    this->InitUp = up;
    //    this->InitFront = front;
    //    this->Size = size;
    //    this->Position = position;
    //    this->Yaw = yaw;
    //    this->Pitch = pitch;
    //    this->PositionOffset = 0;
    //    this->updateCameraVectors();
    //}

    // Constructor with vectors
    Camera(GLfloat size, glm::vec3 aim = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat distance = 1.0f,
        glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 front = glm::vec3(-1.0f, 0.0f, 0.0f),
        GLfloat yaw = YAW, GLfloat pitch = PITCH, GLfloat position_offset = 0)
        : MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), ScrollSensitivity(SSENSITIVTY), Zoom(ZOOM)
    {
        up = glm::normalize(up); front = glm::normalize(front);
        this->InitUp = up;
        this->InitFront = front;
        this->Size = size;
        
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->PositionOffset = 0;
        this->updateCameraVectors();
        this->Position = aim - distance*this->Front;
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        //qDebug() << "pitch:" << this->Pitch;
        //qDebug() << "yaw:" << this->Yaw;
        return glm::lookAt(this->Position+this->Front*this->PositionOffset, (this->Position + this->Front * this->PositionOffset + this->Front), this->Up);
        
        //qDebug() << "pitch:" << this->Pitch;
    }

    void setPosition(glm::vec3 aim = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat distance = 1.0f) {
        this->Position = aim - distance * this->Front;
    }
    void setScrollSensitivity(float input) {
        this->ScrollSensitivity = input;
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
    }
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessAttitudeMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }
    void ProcessAttitudeMovementByPoint(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true, glm::vec3 target = { 0.0f, 0.0f, 0.0f })
    {
        glm::vec3 p_vec = this->Position - target;
        GLfloat distance = -glm::dot(p_vec , this->Front);
        glm::vec3 p_offset = p_vec + distance * this->Front;

        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;
        if (this->Yaw < -180.0f) {
            this->Yaw += 360.0f;
        }
        if (this->Yaw >= 180.0f) {
            this->Yaw -= 360.0f;
        }

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > 90.0f)
                this->Pitch = 90.0f;
            if (this->Pitch < -90.0f)
                this->Pitch = -90.0f;
        }
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
        this->Position = target + p_offset - distance * this->Front;
    }
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessPositionMovement(GLfloat xoffset, GLfloat yoffset)
    {
        xoffset *= this->MouseSensitivity*this->Size/100.0f;
        yoffset *= this->MouseSensitivity*this->Size/100.0f;

        this->Position += xoffset * this->Right;
        this->Position += yoffset * this->Up;
    }
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset)
    {
        this->Size += yoffset * this->ScrollSensitivity;
        if (this->Size <= 0) {
            this->Size = 1;
        }
       
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        glm::mat4 tMatrix = glm::mat4(1.0f);
        tMatrix = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(Yaw), InitUp);
        this->Right = tMatrix * glm::vec4(glm::cross(this->InitFront, this->InitUp) , 1.0f);
        this->Front = tMatrix * glm::vec4(this->InitFront, 1.0f);
        tMatrix = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(Pitch), this->Right);
        this->Front = tMatrix * glm::vec4(this->Front, 1.0f);

        this->Right = glm::normalize(this->Right);
        this->Front = glm::normalize(this->Front);
        this->Up = glm::cross(this->Right, this->Front);

        //front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        //front.y = sin(glm::radians(this->Pitch));
        //front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        //this->Front = glm::normalize(front);
        //// Also re-calculate the Right and Up vector
        //this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }

};