#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <GL/glut.h>
#include <fstream>

//参考: https://wiki.wxwidgets.org/WxGLCanvas
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

//OpenGL メイン処理
void wxGLCanvasSubClass::Render()
{
    static const GLfloat light_pos[4] = { 4, 4, 4 ,1};//アフィン変換用の座標系;長さ4
    static auto glc = wxGLContext(this);

    int id = 0;

    wxPaintDC(this);
    SetCurrent(glc);

    glEnable(GL_DEPTH_TEST);//フラグ有効化
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//2個目のフラグ:隠れ面削除



    glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);//ビューポート変換；サイズ正規化

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();// 行列を初期化
    gluPerspective(60.0, (GLdouble)GetSize().x / (GLdouble)GetSize().y, 0.5, 10);//パース(遠近)設定

    glMatrixMode(GL_MODELVIEW);// モデルビュー行列の設定
    glLoadIdentity();// 行列を初期化
    gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//カメラ設定;斜め上から見下ろす角度
    

    glRotatef(roll, 0.0, 1.0, 0.0);//回転処理
    glRotatef(pitch, 1.0, 0.0, 0.0);

    //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);//光源(LIGHT0)設定



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