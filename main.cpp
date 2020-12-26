#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include <iostream>
#include <fstream>
#include<queue>
#include "Huff.h"

#define NUMBER 200

using namespace std;

struct char_info
{
    char _ch;//下标
    int weight;
    char* _code;

};

char_info info[NUMBER];
char option;
int numberOfLeafNode = 0;
int nodeIndex = 0;
HuffTree<char>* root;
void Stat(char* s, int* w, int& num);
HuffTree<char>* HuffmanBuild(char* s, int* w, int num);
void HuffmanCode(HuffNode<char>* ht1, char* Code, int length);
void char_code();
void File_Code();
char_info getChInfo(char ch);
void File_Decode();


int main() {




    
    
    do {
        cout << "操作命令说明：" << endl;
        cout << "统计输入文件字符频度并对字符集编括码并输出至文件：1" << endl;
        cout << "  对整个文件编括码并保存编码结果到一个二进制文件：2" << endl;
        cout << "            文件解码并将解码结果保存为一文本文件：3" << endl;
        cout << "                                           退出： 4" << endl;
        printf("\n$$");
        scanf("%c", &option);       //用户选择操作命令键入
        switch (option) {        //根据用户选择调用相关函数完成指定操作
        case '1':             //基本要求，输入文件，统计，编码字符集并输出
            char_code();
            system("pause");
            break;
        case '2':            //中级要求，编括码整个文件并保存至一个二进制文件
            File_Code();
            system("pause");
            break;
        case '3':     //高级要求，  解码并保存解码结果到一个文本文件，以便与原始的文件作对比
            File_Decode();
            system("pause");
            break;
        default:
            break;
        }
    } while (option != '4');

    return 0;
}

//读入文本文件并统计频度不为零的字符集s，相应频度集 w，及个数num
void Stat(char* s, int* w, int& num) {
    string tgFile;
    char ch;   
    //getchar();
    cout << "请输入待编码文件名（.txt）:";
    cin >> tgFile;


    ifstream infile;
    infile.open(tgFile, ios::in);

    while (!infile) {      
        if (!infile) {
            cout << "打开" + tgFile + "失败！" << endl;
            cout << "请重新输入正确的待编码文件名（.txt）:";
            getline(cin, tgFile, '\n');
            infile.open(tgFile, ios::in);
        }
    }
    infile.read(&ch, 1);
    while (!infile.eof()) {
        if (ch != '\0') {              
            w[ch]++;
            s[ch] = ch;
        }
        infile.read(&ch, 1);
    }
    int i = 0;
    while (i < NUMBER) {       
        if (w[i] != 0) {
            num++;
        }
        i++;
    }
    numberOfLeafNode = num;
    infile.close();
}

void char_code() {
    
    int n=0;
    char* Code;
    char s[NUMBER] = {'\0'};
    int w[NUMBER] = {0};
    Stat(s, w, n);
    root = HuffmanBuild(s, w, n);
    
   // HuffmanTree->print();
    cout << endl << "the coding result is:" << endl;
    Code = new char[n] {'\0'};
    
    HuffmanCode(root->root(), Code, -1);
}

void HuffmanCode(HuffNode<char>* ht, char* Code, int length) {
    if (ht == nullptr) return;
    if (ht->isLeaf()) {


        if (ht->val() != '\n') {
        printf("%c%5d%30s\n", ht->val(),
            ht->weight(),
            Code);
        }
        else {
            printf("\\n%4d%30s\n",
                ht->weight(),
                Code);
        }

        if (option == '2') {
            info[nodeIndex]._ch = ht->val();
            info[nodeIndex].weight = ht->weight();
            info[nodeIndex]._code = new char[numberOfLeafNode] {0};
            strcpy(info[nodeIndex]._code,Code);
            nodeIndex++;
        }


        return;
    }
    if (ht->left() != nullptr) {
        int len = length;
        len++;
        char temp[30] = {'\0'};
        strcpy(temp, Code);
        temp[len] = '0';
        HuffmanCode(ht->left(), temp, len);
    }
    if (ht->right() != nullptr) {
        int len = length;
        len++;
        char temp[30] = {'\0'};
        strcpy(temp, Code);
        temp[len] = '1';
        HuffmanCode(ht->right(), temp, len);
    }
}

HuffTree<char>* HuffmanBuild(char* s, int* w, int num) {

   priority_queue<HuffNode<char>*, vector<HuffNode<char>*>, Cmp> a;
  

    for (int i = 0; i < NUMBER; i++) {
        if (w[i] != 0) { 
            a.push(new HuffNode<char>(s[i],w[i]));    
        }
    }

    while(a.size()>1){
        HuffNode<char>*f =new HuffNode<char>();
        f = a.top();
        a.pop();
        HuffNode<char>*s = new HuffNode<char>();
        s = a.top();
        a.pop();
        HuffNode<char>*tmp = new HuffNode<char>();
        tmp->setWgt(f->weight() + s->weight());
        tmp->setLeft(f);
        tmp->setRight(s);
        a.push(tmp);
    }

    HuffTree<char>* hufftree = new HuffTree<char>(a.top());
    return hufftree;
   
    return NULL;
}

void File_Code() {
    nodeIndex = 0;
    char_code();
    int pos = 0;
    char value = 0;


    char tgFile[20];
    char outFile[20];
    char ch;

    cout << "请输入待压缩文件名(.txt):" << endl;
    cin >> tgFile;
    FILE* ifp = fopen(tgFile, "rb");
    if (ifp == nullptr) {
        cout << "文件打开失败，请重试" << endl;
        system("pause");
        return;
    }
    cout << "请为压缩文件命名(.huffman):" << endl;
    cin >> outFile;
    FILE* ofp = fopen(outFile, "wb");
    if (ofp == nullptr) {
        cout << "文件加载失败，请重试" << endl;
        system("pause");
        return;
    }
    ch = fgetc(ifp);

    while (!feof(ifp)) {
        if (ch == '\r') {
            ch = fgetc(ifp);
        }
        string code = (string)getChInfo(ch)._code;

        for (unsigned int i = 0; i < code.size(); i++) {
            value <<= 1;
            if (code[i] == '1') {
                value = value + 1;
            }
            else {
                value = value + 0;
            }
            pos++;
            if (pos == 8) {
                fputc(value, ofp);
                pos = 0;
                value = 0;
            }
        }
        ch = fgetc(ifp);
    }
    if (pos) {
        value <<= (8 - pos);
        fputc(value, ofp);
    }


    int size_i = 0;
    int size_o = 0;
    

    fseek(ifp, 0, SEEK_END);
    size_i = ftell(ifp);
    fseek(ofp, 0, SEEK_END);
    size_o = ftell(ofp);

    double rate = (double)size_o / (double)size_i;

    cout << endl << "压缩率:" << rate << endl;





    fclose(ifp);
    fclose(ofp);

}

void File_Decode() {
    FILE* ifp;
    FILE* ofp;
    char tgFile[20];
    char outFile[20];
    unsigned char ch;
    unsigned char temp;

    HuffNode<char>* cur;
    int pos=7;
    long int charCount = root->weight();
    
    cout << "请输入待解压文件名(.huffman):" << endl;
    cin >> tgFile;
    ifp = fopen(tgFile, "rb");
    if (ifp == nullptr) {
        cout << "文件打开失败，请重试" << endl;
        system("pause");
        return;
    }
    cout << "请为已解压文件命名(.huffman.txt):" << endl;
    cin >> outFile;
    ofp = fopen(outFile, "wb");
    if (ofp == nullptr) {
        cout << "文件加载失败，请重试" << endl;
        system("pause");
        return;
    }



    ch = fgetc(ifp);
    cur = root->root();
    while (!feof(ifp)) {

        while (charCount) {
            if (cur->isLeaf()) {
                charCount--;
                fputc(cur->val(), ofp);
                cur = root->root();
                if (charCount == 0) {
                    fclose(ifp);
                    fclose(ofp);
                    return;
                }
            }
            if (!(cur->isLeaf())) {
                temp = ch;
                temp = temp >> pos;
                if (temp % 2 == 0)
                    cur = cur->left();
                else
                    cur = cur->right();
                pos--;
                if (pos < 0) {
                    pos = 7;
                    ch = fgetc(ifp);
                }
            }
            
        }
        

    }

    fclose(ifp);
    fclose(ofp);


}





char_info getChInfo(char ch) {
    int i = 0;
    char_info error = {};
    for (i = 0; i < numberOfLeafNode; i++) {
        if (info[i]._ch == ch) {
            return info[i];
        }
    }
    return error;
}
