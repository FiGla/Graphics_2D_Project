#include <windows.h>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;
PAINTSTRUCT ps ;

union OutCode{
    unsigned All:4;
    struct{unsigned left:1,top:1,right:1,bottom:1;};
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom){
    OutCode out;
    out.All=0;
    if(x<xleft)out.left=1;else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;else if(y>ybottom)out.bottom=1;
    return out;
}

void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi){
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi){
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom){
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All)){
        double xi,yi;
        if(out1.All){
            if(out1.left) VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top) HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right) VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
      }
      else{
        if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
        else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
        else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
        else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
        x2=xi;
        y2=yi;
        out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    }
}
    if(!out1.All && !out2.All){
        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
        cout << "HELLLOOO \n";
    }
}


struct store{
    double x1,x2,y1,y2;
    double x3,y3,x4,y4;
    string T ;
};

double X1,X2,Y1,Y2,num=0,type;
double R1,R11,R2,R22,R3,R33,R4,R44;
vector <store> todraw ;
double X3,Y3,X4,Y4;

void Write(){
    freopen("Out.txt","w",stdout);
    cout << todraw.size()<< endl;
    for (int i=0 ; i < todraw.size();++i){
        if (todraw[i].T == "Her" || todraw[i].T == "Ber" || todraw[i].T == "Rec")
            cout << todraw[i].T << " " << todraw[i].x1 << " " << todraw[i].x2 << " " << todraw[i].y1 << " " << todraw[i].y2
            << " "<<todraw[i].x3 << " " << todraw[i].x4 << " "<< todraw[i].y3 << " " << todraw[i].y4 << endl;
        else
        cout << todraw[i].T << " " << todraw[i].x1 << " " << todraw[i].x2 << " " << todraw[i].y1 << " " << todraw[i].y2<< endl;
    }
}

void Draw4Points(HDC hdc,double xc,double yc, double a, double b,COLORREF color);
void Draw8Points(HDC hdc,double xc,double yc, double a, double b,COLORREF color);
void CircleDirect(HDC hdc,double xc,double yc, double x2,double y2);
void CirclePolar(HDC hdc,double xc,double yc,  double x2,double y2 );
void CircleIterativePolar(HDC hdc,double xc,double yc, double x2,double y2);
void CircleBresenham(HDC hdc,double xc,double yc, double x2,double y2 );
void CircleFasterBresenham(HDC hdc,double xc,double yc,double x2,double y2);

void DrawBezierCurve(HDC hdc , int x0 , int y0 , int X3 , int Y3 , int x2 ,int y2 , int x3 , int y3);
void DrawHermiteCurve(HDC hdc ,int x0 , int y0 , int r0 , int s0 , int  X3 , int Y3 , int r1 ,int  s1);



void DrawLine(HDC hdc,double xs,double ys,double xe,double ye){
     COLORREF color=RGB(0,0,0);
      SetPixel(hdc,xs,ys,color);
    double dx=xe-xs;
    double dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
    if(abs(dx)>=abs(dy))
    {
    double x=xs,xinc= dx>0?1:-1;// as line may be increasing or decreasing
    double y=ys,yinc=(double)dy/dx*xinc;
    while(x!=xe){
    x+=xinc;
    y+=yinc;
    SetPixel(hdc,x,round(y),color);
    }
    }
      else
    {
    double y=ys,yinc= dy>0?1:-1;
    double x=xs,xinc=(double)dx/dy*yinc;
    while(y!=ye){
    x+=xinc;
    y+=yinc;
    SetPixel(hdc,round(x),y,color);
    }
    }
}

void Rectangle (HDC hdc , double X1,double X2,double X3, double X4, double Y1, double Y2, double Y3, double Y4){
    DrawLine (hdc , X1,Y1,X2,Y2);
    DrawLine (hdc , X2,Y2,X3,Y3);
    DrawLine (hdc , X3,Y3,X4,Y4);
    DrawLine (hdc , X4,Y4,X1,Y1);

}
//=====================Cartisan===============================================
void swap (double &x, double &y ){
		double temp ;
		temp = x;
		x = y ;
		y = temp;
}

void Cartis(HDC hdc,double xs,double ys,double xe,double ye){
    COLORREF color=RGB(150,0,180);
	double deltaX = xe-xs;
	double deltaY = ye-ys;

	if (abs(deltaY) <= abs(deltaX)){
		double slope = (double)deltaY/deltaX;
		if(xs > xe){
			swap(xs,xe);
			swap(ys,ye);
		}
		for (double x =xs ; x <=xe ; ++x){
			double y = ys+(x-xs)*slope;
			SetPixel(hdc,x,round(y),color);
         //   cout << y << " " << round(y) << endl;
		}
	}
	else {
		double slope = (double)deltaX/deltaY;
		if (ys> ye){
			swap (xs,xe);
			swap(ys,ye);
		}
		for (double y=ys ; y <= ye; ++y){
			double x= xs + (y-ys) * slope ;
			SetPixel(hdc, round(x),y , color);
		}
	}
}
//======================= parametric =================
void parametric (HDC hdc,double xs,double ys,double xe,double ye){
    COLORREF color=RGB(48,200,16);
    if (xs > xe){
        swap(xs,xe);
        swap(ys,ye);
    }

    double dx = xe-xs;
    double dy = ye-ys;

    double x=xs , y = ys;
    float ro = max(dx,dy);
    ro = 1/ro;
    for (float i=0 ; i < 1 ; i+= ro){
        x= xs +int (i*dx);
        y= ys +int (i*dy);
        SetPixel(hdc,x,y,color);
    }

}
//=====================Mid point===============================================
double fun(double x , double y , double xs, double ys , double deltaX , double deltaY){
    return -(y-ys)*deltaX + (x-xs)*deltaY;
}
void MidPoint (HDC hdc,double xs,double ys,double xe,double ye){
    COLORREF color=RGB(15,160,90);

	double deltaX = xe-xs;
	double deltaY = ye-ys;
    double slope = (double)deltaY/deltaX;

    double x , y;
    SetPixel(hdc , x , y ,color);
    if(abs(slope ) < 1 ){
        if (xs > xe ){
            swap(xs,xe);
            swap(ys,ye);
            deltaX *=-1;
            deltaY*=-1;
        }
        x = xs , y = ys;
        while (x <= xe){
            double ret = fun(x+1,y+1/2,xs,ys,deltaX,deltaY);
            if ( ret > 0) y+=1;
            else if (ret < 0) y-=1;
            x+=1;
            SetPixel(hdc , x , y ,color);
        }
    }
    else {
        if (ys > ye){
            swap(xs , xe);
            swap (ys, ye);
        }else {
            deltaX*=-1;
            deltaY*=-1;
        }
        x = xs , y = ys;

         while (y <= ye){
            double ret = fun(x+1/2,y+1,xs,ys,deltaX,deltaY);
            if ( ret > 0) x+=1;
            else if (ret < 0) x-=1;
            y+=1;
            SetPixel(hdc , x , y ,color);
        }
    }
}
void CartElipse (HDC hdc , double xc,double yc , double xe , double ye ){
    double B = abs(yc-(xe/xc)*yc);
    double A = abs(xe-xc);
    COLORREF color=RGB(20,60,70);
    double x =0 , y = B;
    Draw4Points(hdc , xc,yc,x,round(y),color);
    while (x*pow(B,2) <= y* pow(A,2)){
        x++;
        y = B* sqrt(1-pow(x,2)/pow(A,2));
        Draw4Points(hdc , xc,yc,x,round(y),color);
    }
    x=A,y=0;
    Draw4Points(hdc , xc,yc,x,y,color);
    while (x*pow(B,2) > y* pow(A,2)){
        y++;
        x = A* sqrt(1-pow(y,2)/pow(B,2));
        Draw4Points(hdc , xc,yc,round(x),y,color);
    }

}
void PolElipse (HDC hdc , double xc,double yc , double xe , double ye ){
    double B = abs(yc-(xe/xc)*yc);
    double A = abs(xe-xc);
    double deltaCeta = 1/max(B,A);
    double ceta =0;
    COLORREF color=RGB(20,200,70);

    double x =A , y = 0;
    Draw4Points(hdc , xc,yc,x,round(y),color);
    while (x>0){
        ceta = ceta + deltaCeta;
        x = A*cos(ceta);
        y = B*sin(ceta);
        Draw4Points(hdc , xc,yc,x,round(y),color);
    }
}

void read(HDC hdc){
    freopen("Out.txt","r",stdin);
    int sz ;
    cin >> sz ;
    for ( int i=0 ; i < sz ; ++i ){
        double fir,en , fir2,en2,fir3,en3,fir4,en4;
        string ty;
        cin >>ty>> fir >> fir2 >> en >> en2  ;
        if (ty == "Faster") CircleFasterBresenham(hdc,fir,en,fir2,en2);
        if (ty == "Bres") CircleBresenham(hdc,fir,en,fir2,en2);
        if (ty == "Iterative") CircleIterativePolar(hdc,fir,en,fir2,en2);
        if (ty == "Polar") CirclePolar(hdc,fir,en,fir2,en2);
        if (ty == "Direct") CircleDirect(hdc,fir,en,fir2,en2);

        if (ty == "Mid") MidPoint(hdc,fir,en,fir2,en2);
        if (ty == "Par") parametric(hdc,fir,en,fir2,en2);
        if (ty == "Car") Cartis(hdc,fir,en,fir2,en2);

        if (ty == "CartElipse")CartElipse(hdc,fir,en,fir2,en2);
        if (ty == "PolElipse")PolElipse(hdc,fir,en,fir2,en2);

        if (ty == "Her"){
            cin >> fir3 >> fir4 >> en3 >> en4 ;
            DrawHermiteCurve(hdc,fir,en,fir2,en2,fir3,en3,fir4,en4);
        }

        if (ty == "Ber"){
            cin >> fir3 >> fir4 >> en3 >> en4 ;
            DrawBezierCurve(hdc,fir,en,fir2,en2,fir3,en3,fir4,en4);
        }

        if (ty == "Rec"){
            cin >> fir3 >> fir4 >> en3 >> en4 ;
            Rectangle(hdc,fir,fir2,fir3,fir4,en,en2,en3,en4);
        }
    }

}

//==============================================================================
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           544,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
        HDC hdc;

    switch (message)                  /* handle the messages */
    {

        case WM_CREATE :{
            HMENU hMenu = CreateMenu();
            HMENU Lines = CreateMenu();
            HMENU Circle = CreateMenu();
            HMENU Sav = CreateMenu();
            HMENU Elip = CreateMenu();
            HMENU Cur = CreateMenu();
            HMENU Rec = CreateMenu();
            HMENU Clip = CreateMenu();



            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Lines, "Line Algorithms");
            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Circle, "Circle Algorithms");
            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Elip, "Ellipse");
            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Cur, "Curve");
            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Rec, "Rectangle");
            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Clip, "Clipping");
            AppendMenu (hMenu, MF_POPUP, (UINT_PTR)Sav, "File");



            AppendMenu (Lines, MF_STRING, 1, "Cart");
            AppendMenu (Lines, MF_STRING, 2, "Parametric");
            AppendMenu (Lines, MF_STRING, 3, "MidPoint");

            AppendMenu (Circle, MF_STRING, 4, "Direct");
            AppendMenu (Circle, MF_STRING, 5, "Polar");
            AppendMenu (Circle, MF_STRING, 6, "Iterative Polar");
            AppendMenu (Circle, MF_STRING, 7, "Bresenham");
            AppendMenu (Circle, MF_STRING, 8, "Faster Bresenham");

            AppendMenu (Elip, MF_STRING, 11, "Cart ");
            AppendMenu (Elip, MF_STRING, 12, "Polar");


            AppendMenu (Cur, MF_STRING, 13, "Ber");
            AppendMenu (Cur, MF_STRING, 14, "Her");

            AppendMenu (Rec, MF_STRING, 15, "Rectangle");

            AppendMenu (Clip, MF_STRING, 16, "Clipping");

            AppendMenu (Sav, MF_STRING, 9, "Save");
            AppendMenu (Sav, MF_STRING, 10, "Load");

            SetMenu(hwnd,hMenu);
            break;
        }
        case WM_COMMAND:{
            if(HIWORD(wParam) == 0){
                int menuID = LOWORD(wParam);
                switch(menuID){
                    case 1:
                        type=1;
                        break;
                    case 2:
                        type=2;
                        break;
                    case 3:
                        type=3;
                        break;
                    case 4:
                        type=4;
                        break;
                    case 5:
                        type=5;
                        break;
                    case 6:
                        type=6;
                        break;
                    case 7:
                        type=7;
                        break;
                    case 8:
                        type=8;
                        break;
                    case 9:
                        type=9;
                        break;
                     case 10:
                        type=10;
                        break;
                     case 11:
                        type = 11;
                        break;
                     case 12:
                        type = 12;
                        break;
                     case 13:
                        type = 13;
                        break;
                     case 14 :
                        type = 14 ;
                        break;
                    case 15  :
                        type = 15 ;
                        break;
                    case 16  :
                        type = 16 ;
                        break;
                }
            }
    }
        case WM_LBUTTONDOWN:{
			hdc=GetDC(hwnd);

            if ( num == 0){
                 X1  = LOWORD(lParam);
                 Y1 = HIWORD(lParam);
                if (type == 9){
                    Write();
                    num=0;
                }
                if (type == 10){
                    read(hdc);
                    num=0;
                }

                 ++num;
			 }
			 else if (num == 1){
                store Draw;
                X2  = LOWORD(lParam);
                Y2 = HIWORD(lParam);
                Draw.x1=X1;Draw.x2=X2;Draw.y1=Y1;Draw.y2=Y2;
                if (type == 8){
                    CircleFasterBresenham(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Faster";
                    }
                if (type == 7){
                    CircleBresenham(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Bres";
                }
                if(type == 6){
                    CircleIterativePolar(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Iterative";
                }
                if (type == 5){
                    CirclePolar(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Polar";

                }
                if (type == 4){
                    CircleDirect(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Direct";

                }
                if(type == 3){
                    MidPoint(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Mid";

                }
                if (type == 2){
                    parametric(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Par";
                }
                if (type == 1){
                    Cartis(hdc,X1,Y1,X2,Y2);
                    Draw.T = "Car";
                }
                if(type == 11){
                    CartElipse(hdc,X1,Y1,X2,Y2);
                    Draw.T = "CartElipse";
                }
                if(type == 12){
                    PolElipse(hdc,X1,Y1,X2,Y2);
                    Draw.T = "PolElipse";
                }
                if (type == 13 || type == 14 || type == 15 || type == 16) ++num;
                else {
                    num=0;
                    todraw.push_back(Draw);
                }


			 }
			 else if (num == 2){
                X3  = LOWORD(lParam);
                Y3 = HIWORD(lParam);
                ++num;
            }
            else if (num == 3){
                store Draw;
                X4  = LOWORD(lParam);
                Y4 = HIWORD(lParam);
                Draw.x1=X1,Draw.x2=X2,Draw.x3=X3,Draw.x4=X4,Draw.y1=Y1,Draw.y2=Y2,Draw.y3=Y3,Draw.y4=Y4;
                if (type == 13 ){
                        DrawBezierCurve(hdc,X1,Y1,X2,Y2,X3,Y3,X4,Y4);
                        Draw.T = "Ber";
                }
                if (type == 14 ){
                        DrawHermiteCurve(hdc,X1,Y1,X2,Y2,X3,Y3,X4,Y4);
                        Draw.T ="Her";
                }
                if (type == 15 ){
                        Rectangle(hdc,X1,X2,X3,X4,Y1,Y2,Y3,Y4);
                        Draw.T = "Rec";
                }
                if (type == 16 ){
                    CohenSuth(hdc,X1,Y1,X2,Y2,X3,Y3,X4,Y4);

                }
                todraw.push_back(Draw);
                num =0;
            }

/* rectangle

            if (num == 0 ){
                R1  = LOWORD(lParam);
                R11 = HIWORD(lParam);
                ++num;
            }else if (num == 1 ){
                R2  = LOWORD(lParam);
                R22 = HIWORD(lParam);
                ++num;
            }else if (num == 2 ){
                R3  = LOWORD(lParam);
                R33 = HIWORD(lParam);
                ++num;
            }else if (num == 3 ){
                R4  = LOWORD(lParam);
                R44 = HIWORD(lParam);
                cout << "HELLO   \n";
                Rectangle(hdc,R1,R2,R3,R4,R11,R22,R33,R44);
                num=0;

            }
*/

			ReleaseDC(hwnd,hdc);
			break;
		}

		case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
/*****************************************************************************************************/

void Draw4Points(HDC hdc,double xc,double yc, double a, double b,COLORREF color){
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc-a, yc-b, color);

}
void Draw8Points(HDC hdc,double xc,double yc, double a, double b,COLORREF color){
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}
//==================CircleDirect=================================
void CircleDirect(HDC hdc,double xc,double yc, double x2,double y2){
    COLORREF color=RGB(100,0,20);
    double x1 = (x2 - xc)*(x2 - xc);
    double y1 = (y2 - yc)*(y2 - yc);
    double R = sqrt(x1+y1);
    double x=0,y=R;
    double R2=R*R;

    if(xc > x2){
        swap(xc,x2);
        swap(yc,y2);
    }
    Draw8Points(hdc,xc,yc,x,y,color);

    while(x<y){
        x++;
        y=round(sqrt((double)(R2-x*x)));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
//==================CirclePolar=================================
void CirclePolar(HDC hdc,double xc,double yc,  double x2,double y2 ){
    COLORREF color=RGB(150,50,40);
    double x1 = (x2 - xc)*(x2 - xc);
    double y1 = (y2 - yc)*(y2 - yc);
    double R = sqrt(x1+y1);
    double x=R,y=0;
    double theta=0,dtheta=1.0/R;

    if(xc > x2){
        swap(xc,x2);
        swap(yc,y2);
    }
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x>y){
        theta+=dtheta;
        x=round(R*cos(theta));
        y=round(R*sin(theta));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
//==================CircleIterativePolar=================================
void CircleIterativePolar(HDC hdc,double xc,double yc, double x2,double y2){
    COLORREF color=RGB(20,60,70);
    double x1 = (x2 - xc)*(x2 - xc);
    double y1 = (y2 - yc)*(y2 - yc);
    double R = sqrt(x1+y1);
    if(xc > x2){
        swap(xc,x2);
        swap(yc,y2);
    }
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    Draw8Points(hdc,xc,yc,R,0,color);
    while(x>y){
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;
        Draw8Points(hdc,xc,yc,round(x),round(y),color);
    }
}
//==================CircleBresenham=================================
void CircleBresenham(HDC hdc,double xc,double yc, double x2,double y2 ){
    COLORREF color=RGB(200,190,150);
    double x1 = (x2 - xc)*(x2 - xc);
    double y1 = (y2 - yc)*(y2 - yc);
    double R = sqrt(x1+y1);
    double x=0,y=R;
    double d=1-R;

    if(xc > x2){
        swap(xc,x2);
        swap(yc,y2);
    }

    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y){
        if(d<0)
            d+=2*x+2;
        else{
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
//==================CircleFasterBresenham=================================

void CircleFasterBresenham(HDC hdc,double xc,double yc,double x2,double y2){
    COLORREF color=RGB(120,200,150);

    double x1 = (x2 - xc)*(x2 - xc);
    double y1 = (y2 - yc)*(y2 - yc);
    double R = sqrt(x1+y1);
    double x=0,y=R;
    double d=1-R;
    double c1=3, c2=5-2*R;

    if(xc > x2){
    swap(xc,x2);
    swap(yc,y2);
    }

    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0){
            d+=c1;
            c2+=2;
        }
        else{
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
        }
}

void DrawHermiteCurve(HDC hdc ,int x0 , int y0 , int r0 , int s0 , int  X3 , int Y3 , int r1 ,int  s1){
 COLORREF color=RGB(255,0,150);
    int H[4][4]=
    {
        {1,0,0,0},
        {0,1,0,0},
        {-3,-2,3,-1},
        {2,1,-2,1}
    };
    double vx[4]= {x0,r0,X3,r1};
    double vy[4]= {y0,s0,Y3,s1};
    double Cx[4];
    double Cy[4];
    float x;
    float y;
    double dt=0.0005;


    for(int i=0; i<4; i++)
    {
        double result=0.0;
        for(int j=0; j<4; j++)
        {

            result+=H[i][j]*vx[j];
        }
        Cx[i]=result;
    }
    for(int i=0; i<4; i++)
    {
        double result=0;
        for(int j=0; j<4; j++)
        {

            result+=H[i][j]*vy[j];
        }
        Cy[i]=result;
    }



    for(int t=0; t<1.0; t+= dt)
    {
        double  T[4]= {1,t,t*t,t*t*t};
        x=0 , y=0;
        for(int j=0; j<4; j++)
        {

            x+=Cx[j]*T[j];
        }
        for(int j=0; j<4; j++)
        {

            y+=Cy[j]*T[j];
        }
        SetPixel(hdc,round(x),round(y),color);
    }

}

/*****************************************************************************/

void DrawBezierCurve(HDC hdc , int x0 , int y0 , int X3 , int Y3 , int x2 ,int y2 , int x3 , int y3){
COLORREF color=RGB(0,0,255);
 int B[4][4]=
    {
        {1,0,0,0},
        {-3,3,0,0},
        {3,-6,3,0},
        {-1,3,-3,1}

    };
    double vx[4]= {x0,X3,x2,x3};
    double vy[4]= {y0,Y3,y2,y3};
    double Cx[4];
    double Cy[4];
    float x;
    float y;
    double dt = 0.0005;

    for(int i=0; i<4; i++)
    {
        double result=0;
        for(int j=0; j<4; j++)
        {

            result+=B[i][j]*vx[j];


        }
        Cx[i]=result;
    }
    for(int i=0; i<4; i++)
    {
        double result=0;
        for(int j=0; j<4; j++)
        {

            result+=B[i][j]*vy[j];
        }
        Cy[i]=result;
    }
    for(double t=0; t<1.0; t+= dt)
    {
        double  T[4]= {1,t,t*t,t*t*t};
        x=0 , y=0;
        for(int j=0; j<4; j++)
        {

            x+=Cx[j]*T[j];
        }
        for(int j=0; j<4; j++)
        {

            y+=Cy[j]*T[j];
        }
        SetPixel(hdc,round(x),round(y),color);
    }
}
