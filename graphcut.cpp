// ƒø«∞“—æ≠µƒµΩ¡Àµ⁄“ª≤Ω÷Æ∫Ûµƒa£¨b ˝◊È∑÷±¥˙±Ì¡Àµ„µƒ±Í«© «S£¨Tµƒ∏≈¬ £¨‘⁄Ω®Õºµƒ ±∫Ú“™◊¢“‚Ω¯––œ‡”¶µƒ∏¸∏ƒ°£
// Pij[0,1]“≤“—æ≠µ√µΩ£¨¥˙±Ì¡Ωµ„≤ª¡¨–¯µƒ¥˙º€£¨Ω®Õº ±◊‹µƒƒ‹¡ø∫Ø ˝ø…“‘÷±Ω”º”…œPij
// E = °∆
   
#include "graphcut.h"
//double log(double x);    to calc ln(x)
// be careful about the float and double

using namespace std;
using namespace cv;

//extern const int maxr, maxc, MAXINF;     //MAXINF?
extern int picrow, piccol;     // picrow: the pixel number of a row,  piccol: the pixel number of a col
extern int R[][maxc], G[][maxc], B[][maxc], Gray[][maxc];    //the input img's RGB
extern double afg[][maxc], bbg[][maxc], p[][maxc][2];  //a and b, respectly, are the parameters that refer to the posibility of the pixel belong to S or T
extern int S, T;
extern bool myf[];    // which means the flow we find

const int blue = 0, green = 1, red = 2;

//remember to find if the picture if 8 bits or 16 bits or 32 bits
//we need 8 bits to operate
std::vector<Mat> channels;
Mat Rch, Gch, Bch, Graymat;
void formRGBandGray(Mat & img, Mat & Grayimg)
{
    picrow = img.rows;
    piccol = img.cols;
    split(img, channels);
    Bch = channels.at(blue);
    Gch = channels.at(green);
    Rch = channels.at(red);
    Graymat = Grayimg;
    for (int i = 0; i < picrow; ++i)
    {
        uchar* dataB = Bch.ptr<uchar>(i);
        uchar* dataG = Gch.ptr<uchar>(i);
        uchar* dataR = Rch.ptr<uchar>(i);
        uchar* dataGray = Graymat.ptr<uchar>(i);
        for (int j = 0; j < piccol; ++j)
        {
            B[i][j] = *dataB, dataB++;
            G[i][j] = *dataG, dataG++;
            R[i][j] = *dataR, dataR++;
            Gray[i][j] = *dataGray, dataGray++;
        }
    }
}

//float EulerDis_RGBSpace()       //need to be done
//{
//}

float EulerDis_Gray(double G1, double G2)
{
    return abs(G1 - G2);
}

//float Dis_RGB(double Bx, double Gx, double Rx, double By, double Gy, double Ry)
//{
//}

inline double sqr(double x)
{
    return x*x;
}

bool OnLine(uchar* x, uchar* y)   //true means yes
{
    return (*x - *y) > 25 || (*y - *x) > 25;
}

double avefgc[3] = { 0 }, avebgc[3] = { 0 }, avefgG = 0, avebgG = 0;
int fg_pix_num = 0, bg_pix_num = 0;

void makeAB(Mat srcimg, Mat fgimg, Mat bgimg, Mat Graymat) //Graymat:the Gray of srcimg
{
    fg_pix_num = bg_pix_num = 0;
    int rr = srcimg.rows, cc = srcimg.cols * srcimg.channels();
    uchar *srcp = NULL, *fgp = NULL, *bgp = NULL, *Grayp = NULL;
    for (int i = 0; i < rr; ++i)
    {
        srcp = srcimg.ptr<uchar>(i);
        fgp = fgimg.ptr<uchar>(i);
        bgp = bgimg.ptr<uchar>(i);
        Grayp = Graymat.ptr<uchar>(i);
        for (int j = 0; j < cc; Grayp++, j += 3, srcp += 3, fgp += 3, bgp += 3)
        {
            if (OnLine(srcp, fgp) || OnLine(srcp + 1, fgp + 1) || OnLine(srcp + 2, fgp + 2))
            {//here means the pixel is on the fgline
                fg_pix_num++;
                avefgG += *Grayp;
                //    avefgc[blue] += *fgp;
                //    avefgc[green] += *(fgp + 1);
                //    avefgc[red] += *(fgp + 2);
                afg[i][j / 3] = MAXINF;
                bbg[i][j / 3] = 0;
            }
            else if (OnLine(srcp, bgp) || OnLine(srcp + 1, bgp + 1) || OnLine(srcp + 2, bgp + 2))
            {//here means the pixel is on the bgline
                bg_pix_num++;
                avebgG += *Grayp;
                //    avebgc[blue] = *bgp;
                //    avebgc[green] = *(bgp + 1);
                //    avebgc[red] = *(bgp + 2);
                bbg[i][j / 3] = MAXINF;
                afg[i][j / 3] = 0;
            }
        }
    }

    avefgG /= fg_pix_num;
    avebgG /= bg_pix_num;

    //for(int i = 0; i < 3; ++i)
    //{
    //      avefgc[i] /= fg_pix_num;
    //      avebgc[i] /= bg_pix_num;
    //}

    for (int i = 0; i < picrow; ++i)
    for (int j = 0; j < piccol; ++j)
    {
        if (afg[i][j] >= MAXINF || bbg[i][j] >= MAXINF)
            continue;
        //float disfg = Dis_RGB(B[i][j], G[i][j], R[i][j], avefgc[blue], avefgc[green], avefgc[red]);
        //float disbg = Dis_RGB(B[i][j], G[i][j], R[i][j], avebgc[blue], avebgc[green], avebgc[red]);
        float disfg = EulerDis_Gray(Gray[i][j], avefgG);
        float disbg = EulerDis_Gray(Gray[i][j], avebgG);
        afg[i][j] = disbg / (disfg + disbg);
        bbg[i][j] = disfg / (disfg + disbg);
    }
}

const double delta = 1;
void makePij()  // get the Pij    p(point1,point2) = exp(-(Gray1 - Gray2)^2/2) 
{//  double exp(double x)
    for (int i = 0; i < picrow - 1; ++i)
    for (int j = 0; j < piccol - 1; ++j)
    {
        p[i][j][0] = exp(-sqr((Gray[i][j] - Gray[i + 1][j]) / delta) / 2);
        p[i][j][1] = exp(-sqr((Gray[i][j] - Gray[i][j + 1]) / delta) / 2);
    }
}

void cutbgpixel(Mat & output)
{
    for (int i = 0; i < picrow; ++i)
    {
        uchar* p = output.ptr<uchar>(i);
        for (int j = 0; j < piccol; ++j)
        {
            if (myf[getnum(i, j)])
            {
                *(p + j * 3) = 0;
                *(p + j * 3 + 1) = 0;
                *(p + j * 3 + 2) = 0;
            }
        }
    }
}

int main()
{
    freopen("out.txt", "w", stdout);
    const char* srcname = "in.bmp";
    const char* fgname = "fg.bmp";
    const char* bgname = "bg.bmp";

    Mat srcimg = imread(srcname);
    Mat srcGrayimg = imread(srcname, 0);
    Mat srcfg = imread(fgname);
    Mat srcbg = imread(bgname);

    formRGBandGray(srcimg, srcGrayimg);
    makeAB(srcimg, srcfg, srcbg, srcGrayimg);
    makePij();
    printf("finish makePij()\n");
    build();        //here
    printf("finish build()\n");
    solve();
    printf("finish solve()\n");
    findf();
    printf("finish findf()\n");
    cutbgpixel(srcimg);
    printf("finish cut()\n");

    //???blur
    imshow("output", srcimg);
    cvWaitKey();
    return 0;
}
