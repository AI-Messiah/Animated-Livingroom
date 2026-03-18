#include "VolumeControl.h"

VolumeControl::~VolumeControl()
{
}

void VolumeControl::init()
{
    float vertices[12][4] = {
                { .5,     -.6,     0.0f, 1.0f },
                { .5,     -.8,              0.0f, 0.0f },
                { .8, -.8,          .9f, 0.0f },

                { .5,     -.6,     0.0f, 1.0f },
                { .8, -.8,          .9f, 0.0f },
                { .8, -.6, .9f, 1.0f },
                { xval + .508,     -.7,     0.9f, 1.0f },
                { xval + .508,     -.78,              0.9f, 0.0f },
                { xval + .55, -.78,          1.0f, 0.0f },
                 
                { xval + .508,     -.7,     0.9f, 1.0f },
                { xval + .55, -.78,          1.0f, 0.0f },
                { xval + .55, -.7,  1.0f, 1.0f }
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * 4 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void VolumeControl::drag(float x, float y)
{
    if (x > .5 && x < .8 && y > -.8 && y < -.69) {
        if (oldx < 0) {
            oldx = x;
        }
        else {
            xval += x - oldx;
            oldx = x;
            if (xval < 0) xval = 0;
            if (xval > .242) xval = .242;
            volume = xval / .242;

            float vertices[12][4] = {
                { .5,     -.6,     0.0f, 1.0f },
                { .5,     -.8,              0.0f, 0.0f },
                { .8, -.8,          .9f, 0.0f },

                { .5,     -.6,     0.0f, 1.0f },
                { .8, -.8,          .9f, 0.0f },
                { .8, -.6, .9f, 1.0f },
                { xval + .508,     -.7,     0.9f, 1.0f },
                { xval + .508,     -.78,              0.9f, 0.0f },
                { xval + .55, -.78,          1.0f, 0.0f },

                { xval + .508,     -.7,     0.9f, 1.0f },
                { xval + .55, -.78,          1.0f, 0.0f },
                { xval + .55, -.7,  1.0f, 1.0f }
            };

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 12 * 4 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
            glBindVertexArray(0);
        }
    }
    else {
        oldx = -1;
    }
}

void VolumeControl::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
}

void VolumeControl::reset() {

}