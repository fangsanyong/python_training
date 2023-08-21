// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#ifdef _WIN32
#include <Windows.h>
#include <strsafe.h>
#else
#include <dirent.h>
#endif

#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace cv;
//
//获取指定目录下所有文件的文件名，不包括文件夹，在GetFileFromDir中使用
//strDir: 输入，目录路径
//FileDirList： 输出，文件路径列表
//返回：空
//
vector<string> GetFileNameFromDir(string strDir)
{
    vector<string> vFileDirList;
#ifdef _WIN32
    WIN32_FIND_DATAA ffd;
    //LARGE_INTEGER filesize;
    string szDir;
    //size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;


    szDir = strDir + "\\*.jpg";
    hFind = FindFirstFileA(szDir.c_str(), &ffd);


    if (INVALID_HANDLE_VALUE == hFind)
    {
        cout << "get file name error" << endl;
        return vFileDirList;
    }
    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            string filename = ffd.cFileName;//(const char*)
            string filedir = strDir + "\\" + filename;
            vFileDirList.push_back(filedir);
        }
    } while (FindNextFileA(hFind, &ffd) != 0);


    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        cout << "FindFirstFile error" << endl;
        return vFileDirList;
    }
    FindClose(hFind);
#else
    DIR* dir;
    struct dirent* ptr;
    dir = opendir(strDir.c_str());
    while ((ptr = readdir(dir)) != NULL)
    {
        string filename = string(ptr->d_name);
        if (filename == "." || filename == "..") {
            continue;
        }
        string path = strDir + string("/") + filename;
        vFileDirList.push_back(path);
    }
    closedir(dir);
    sort(vFileDirList.begin(), vFileDirList.end());
#endif

    return vFileDirList;
}

double get_picture_laplacian(string path)
{
    Mat src = imread(path);
    int h = src.rows;
    int w = src.cols;
    Mat src_gray, dst;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    Laplacian(src_gray, dst, CV_64F);
    Mat meanMat, stdMat;
    meanStdDev(dst, meanMat, stdMat);
    double score = stdMat.at<double>(0) / (h * w) ;
    score = score * 1e3;
    return score;
}

double get_picture_scharr(string path)
{
    Mat src = imread(path);
    int h = src.rows;
    int w = src.cols;
    Mat src_gray, dst,dst1;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    Scharr(src_gray, dst, 3, 1, 0);
    Scharr(src_gray, dst1, 3, 0, 1);
    Mat meanMat, stdMat, meanMat1, stdMat1;
    meanStdDev(dst, meanMat, stdMat);
    meanStdDev(dst1, meanMat1, stdMat1);
    double score = (stdMat.at<double>(0)+ stdMat1.at<double>(0)) / (h * w);
    score = score * 1e3;
    return score;

}

double get_picture_tenengrad(string path)
{
    Mat src = imread(path);
    int h = src.rows;
    int w = src.cols;
    Mat src_gray, dst, dst1;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    Sobel(src_gray, dst, 3, 1, 0);
    Sobel(src_gray, dst1, 3, 0, 1);
    Mat meanMat, stdMat, meanMat1, stdMat1;
    meanStdDev(dst, meanMat, stdMat);
    meanStdDev(dst1, meanMat1, stdMat1);
    double score = (stdMat.at<double>(0) + stdMat1.at<double>(0)) / (h * w);
    score = score * 1e3;
    return score;

}

int main()
{

    string strDir = "F:\\5\\qx";
    vector<string> files = GetFileNameFromDir(strDir);
    for (int i = 0; i < files.size(); ++i) {
        //cout << "拉普拉斯算子(Laplacian算子)" << endl;
        double a = get_picture_laplacian(files[i]);
        cout << a << endl;

        //cout << "Scharr(沙尔算子)算子" << endl;
        //double b = get_picture_scharr(files[i]);
        //cout << b << endl;

        //cout << "Tenengrad(Sobel算子)" << endl;
        //double c = get_picture_tenengrad(files[i]);
        //cout << c << endl;
      
    }

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
