#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <GL/glut.h>
#include <fstream>

//�Q�l: https://wiki.wxwidgets.org/WxGLCanvas
class wxGLCanvasSubClass : public wxGLCanvas {
public:
    wxGLCanvasSubClass(wxFrame* parent);
private:
    GLfloat pitch, roll;

    void Render();
    void Paintit(wxPaintEvent& event);
    void OnKey(wxKeyEvent& event);

protected:
};

wxGLCanvasSubClass::wxGLCanvasSubClass(wxFrame* parent)
    :wxGLCanvas(parent, wxID_ANY)
{
    int argc = 1;
    char* argv[1] = { 0 };
    pitch = -2;
    roll = 0;

    this->Connect(wxEVT_PAINT, wxPaintEventHandler(wxGLCanvasSubClass::Paintit));
    this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(wxGLCanvasSubClass::OnKey));

    glutInit(&argc, argv);
}


void wxGLCanvasSubClass::Paintit(wxPaintEvent& event)
{
    Render();
}

//OpenGL ���C������
void wxGLCanvasSubClass::Render()
{
    static const GLfloat light_pos[4] = { 4, 4, 4 ,1};//�A�t�B���ϊ��p�̍��W�n;����4
    static auto glc = wxGLContext(this);

    int id = 0;

    wxPaintDC(this);
    SetCurrent(glc);

    glEnable(GL_DEPTH_TEST);//�t���O�L����
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//2�ڂ̃t���O:�B��ʍ폜



    glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);//�r���[�|�[�g�ϊ��G�T�C�Y���K��

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();// �s���������
    gluPerspective(60.0, (GLdouble)GetSize().x / (GLdouble)GetSize().y, 0.5, 10);//�p�[�X(����)�ݒ�

    glMatrixMode(GL_MODELVIEW);// ���f���r���[�s��̐ݒ�
    glLoadIdentity();// �s���������
    gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//�J�����ݒ�;�΂ߏォ�猩���낷�p�x
    

    glRotatef(roll, 0.0, 1.0, 0.0);//��]����
    glRotatef(pitch, 1.0, 0.0, 0.0);

    //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);//����(LIGHT0)�ݒ�



    //draw cube
    //top plane
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);//cyan
    glTexCoord2f(0, 0);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd();

    //under plane
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);//magenta
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glEnd();
    //side plane(+x)
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);//red
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd();
    //side plane(-x)
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);//red
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();
    //side plane (+y)
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);//blue
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();
    //side plane(-y)
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);//blue
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();




    

    glFlush();
    SwapBuffers();
}

void wxGLCanvasSubClass::OnKey(wxKeyEvent& event)
{
    auto key = event.GetKeyCode();
    switch (key+32)
    {
    case 'w':
        pitch += 5;
        break;
    case 's':
        pitch -= 5;
        break;
    case 'a':
        roll -= 5;
        break;
    case 'd':
        roll += 5;
        break;

    default:
        break;
    }
    this->Refresh();
}


class MyApp : public wxApp
{
    virtual bool OnInit();
};


IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    wxFrame* frame = new wxFrame(NULL, -1, wxT("Hello GL World"), wxPoint(50, 50), wxSize(200, 200));
    new wxGLCanvasSubClass(frame);

    frame->Show(true);
    return true;
}