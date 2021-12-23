#include <iostream>
#include<cstring>
#pragma pack(1)
using namespace std;
struct filehead {
    //存放关键信息
    unsigned char flag[3];				//压缩二进制文件头部标志store huff
    int alphaVariety;		//字符种类
    unsigned char lastValidBit;//最后一个字节的有效位数
    long filelenth;
    unsigned char unused[4];			//待用空间
};
struct huffman
{
    int parent, lchild, rchild;
    int count;
    unsigned char data;
    char *code;//store the huffmancode
    int visit;//visit status
};
struct alphafreq
{
    unsigned char ch;
    long freq;
};
int getlastbit(huffman* huffmantree,int alphavariety);
void select(huffman* huffmana, int n, int* r1, int* r2);
void createhuffmantree(huffman* huffmantree, int alphatypes, int totallenth);
void showmyhuff(huffman* huffmantree, int totallenth);
void createhuffmancode(huffman* huffmantree, int alphatypes);
alphafreq* countalpha(char*sourcename, int& alphavariety);
void showalphafreq(alphafreq* thetable, int alphavariety);
huffman* sethuffmantable(alphafreq* thetable, int alphavariety, int* hufindex);
void huffmancompress(char* source, char* des);
void huf_decode(char* source, char* des);
void clearhuf(huffman* toclear,int alphavariety);
filehead readfilehead(char* source);
alphafreq* readalphafreq(char* source, int& alphavariety, filehead base);
bool checkfile(char* source);
void clearstr(char* str);
int main()
{
    cout << "欢迎使用压缩软件,输入1进行压缩，输入2进行解压缩,0结束程序";
    int level = 0;
    char sourcename[50] = "\0";
    char desname[50] = "\0";
    (cin >> level).get();
    while(level){
        system("cls");
        if (level == 1) {
            cout << "输入要压缩的文件路径+名字:";
            cin >> sourcename;
            
            while (!checkfile(sourcename)) {
                clearstr(sourcename);
                cout << "no exist\n";
                cout << "输入文件路径+地址:";
                cin >> sourcename;
               
            }
            cout << "输入要压缩出来的文件路径+名字:";
            cin >> desname;
            huffmancompress(sourcename, desname);
            clearstr(sourcename);
            clearstr(desname);
        }
    else if (level == 2) {
            cout << "输入要解压的文件路径+名字:";
            cin >> sourcename;
            while (!checkfile(sourcename)) {
                clearstr(sourcename);
                cout << "no exist\n";
                cout << "输入文件路径+地址:";
                cin >> sourcename;
               
            }
            cout << "输入要解压出来的文件路径+名字:";
            cin >> desname;
            huf_decode(sourcename, desname);
            clearstr(sourcename);
            clearstr(desname);
        }
    else {
            cout << "错误的功能选择,重新输入:";
            (cin >> level).get();
        }
        if (level == 1 || level == 2) {
            cout << "你还要干什么:";
            (cin >> level).get();
    }
      
    
    }
     
}
void select(huffman* huffmana, int n, int& r1) {//find the smallest one hasn't been visited
    int i;
    int min1;
    min1 = 0;
    for (i = 0; i < n; i++) {
        if (huffmana[i].visit == 0) {
            min1 = i;
            break;
        }
    }
    for (i = 0; i < n; i++)
    {
        if (huffmana[i].count < huffmana[min1].count && huffmana[i].visit == 0) {
            min1 = i;
        }
    }
    huffmana[min1].visit = 1;//visited
    r1 = min1;
}
/*alphatype是整个文件中的文件类型，huffmantree已经申请好内存并初始化完，totallenth是Huffman树的总长度
*/
void createhuffmantree(huffman* huffmantree, int alphatypes, int totallenth) {
    int min1, min2;
    for (int j = alphatypes; j < totallenth; j++) {
        select(huffmantree, j, min1);
        select(huffmantree, j, min2);
        huffmantree[min1].parent = huffmantree[min2].parent = j;
        huffmantree[j].lchild = min1;
        huffmantree[j].rchild = min2;
        huffmantree[j].count = huffmantree[min1].count + huffmantree[min2].count;

    }
}
//print the huffman table
void showmyhuff(huffman* huffmantree, int totallenth) {
    for (int i = 0; i < totallenth; i++) {
        std::cout << huffmantree[i].count << "\t" << huffmantree[i].lchild << "\t" << huffmantree[i].rchild << "\t" << huffmantree[i].parent <<"\t" << huffmantree[i].code << "\n";
    }
    cout << "finished\n";
}
void createhuffmancode(huffman* huffmantree, int alphatypes) {
    char temp[256];
    for (int i = 0; i < 256; i++) {
        temp[i] = '\0';
    }
    int cur, index, next;
    //from child to parent
    for (int i = 0; i < alphatypes; i++) {
        index = 254;
        for (cur = i, next = huffmantree[cur].parent; next != -1; cur = next, next = huffmantree[next].parent)
        {
            if (cur == huffmantree[next].lchild) {
                temp[index--] = '0';
            }
            else {
                temp[index--] = '1';
            }
        }
        strcpy_s(huffmantree[i].code, alphatypes,&temp[index+1]);
    }
}
int getlastbit(huffman* huffmantree, int alphavariety) {
    int i;
    int sum = 0;
    for (i = 0; i < alphavariety; i++) {
        sum += strlen(huffmantree[i].code) * huffmantree[i].count;
        sum &= 0xFF;
    }
    sum = sum % 8;
    return (sum == 0) ? 8: sum;
}
//对字符种类和每种字符的数量进行计数，返回alphafreq数组，其为ch，freq
alphafreq* countalpha(char* sourcename,int &alphavariety) {
    FILE* fp=fopen(sourcename,  "rb");
    long freq[256] = { 0 };
    unsigned char ch;
    alphavariety = 0;
    fread(&ch, sizeof(unsigned char), 1, fp);
    while (!feof(fp)) {
        freq[ch]++;
        fread(&ch, sizeof(unsigned char), 1, fp);
    }
    fclose(fp);
    //calc the variety
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            alphavariety++;
        }
    }
    alphafreq* thefreq = new alphafreq[alphavariety];
    int i = 0, index = 0;
    for (int i = 0; i < alphavariety; i++) {
        thefreq[i].ch = 0;
        thefreq[i].freq = 0;
    }
    for (; i < 256; i++) {
        if (freq[i]) {
            thefreq[index].ch = i;
            thefreq[index].freq = freq[i];
            index++;
        }
    }
    return thefreq;

}
void showalphafreq(alphafreq* thetable, int alphavariety) {
    for (int i = 0; i < alphavariety; i++) {
        cout << thetable[i].ch<<thetable[i].freq<<"\t";
    }
}
huffman* sethuffmantable(alphafreq* thetable, int alphavariety, int* hufindex) {
    huffman* huffmantable = (huffman*)calloc(2 * alphavariety - 1,sizeof(huffman));
    //create hufindex,used to fast scan
    for (int i = 0; i < alphavariety; i++) {
        hufindex[thetable[i].ch] = i;
        huffmantable[i].lchild = huffmantable[i].rchild = huffmantable[i].parent = -1;
        huffmantable[i].data = thetable[i].ch;
        huffmantable[i].count = thetable[i].freq;
        huffmantable[i].visit = 0;
        huffmantable[i].code = (char*)calloc(sizeof(char), alphavariety);
    }
    for (int i = alphavariety; i < 2 * alphavariety - 1; i++) {
        huffmantable[i].lchild = huffmantable[i].rchild = huffmantable[i].parent = -1;
        huffmantable[i].count = 0;
        huffmantable[i].visit = 0;
        huffmantable[i].code ="\0";
    }
    return huffmantable;

}
void huffmancompress(char* source, char* des) {
    FILE* fin, * fzip;
    alphafreq* freqtable;
    int alphavariety;
    freqtable = countalpha(source, alphavariety);
    huffman* sourcehuf;
    int* hufindex = new int[256];
    for (int i = 0; i < 256; i++) {
        hufindex[i] = 0;
    }
    sourcehuf = sethuffmantable(freqtable, alphavariety, hufindex);
    createhuffmantree(sourcehuf, alphavariety, 2 * alphavariety - 1);
    createhuffmancode(sourcehuf, alphavariety);
    showmyhuff(sourcehuf, alphavariety);



    filehead info = {'h','u','f'};
    info.alphaVariety = alphavariety;
    info.lastValidBit = getlastbit(sourcehuf, alphavariety);
    info.filelenth = 0;
    for (int i = 0; i < alphavariety; i++) {
        info.filelenth += sourcehuf[i].count * sizeof(unsigned char);
    }
    

    fin = fopen(source, "rb");
    fzip = fopen(des, "wb");



    //给文件头部写入元数据
    fwrite(&info, sizeof(filehead), 1, fzip);
    //给元数据后写字符种类和频度，解压缩时需要用这些生成一模一样新的哈夫曼树
    fwrite(freqtable, sizeof(alphafreq),alphavariety, fzip);

    //开始过数据
    unsigned char codebuf;
    int flag = 0;//flag=8 写入文件
    char* hufcode;
    int value = 0;
    codebuf = fgetc(fin);
    while (!feof(fin)) {
        hufcode = sourcehuf[hufindex[codebuf]].code;
        for (int i = 0; hufcode[i]; i++) {
            if (hufcode[i] == '1') {
                value <<= 1;
                value |= 1;
            }
            else {
                value <<= 1;
            }
            flag++;
            if (flag >= 8) {
                fwrite(&value, sizeof(unsigned char), 1,fzip);
                flag = 0;

            }

        }
        codebuf = fgetc(fin);
    }

    if (flag) {
        value <<=(8- info.lastValidBit);
        fwrite(&value, sizeof(unsigned char), 1, fzip);
    }

    fclose(fin);
    fclose(fzip);
    delete sourcehuf;
}



filehead readfilehead(char* source) {
    FILE* toread;
    toread = fopen(source, "rb");
    filehead info;
    fread(&info,sizeof(filehead),1,toread);
    fclose(toread);
    return info;
}
alphafreq* readalphafreq(char* source, int &alphavariety,filehead base) {
    FILE* toread;
    toread = fopen(source, "rb");
    alphavariety = base.alphaVariety;
    alphafreq* result = new alphafreq[alphavariety];
    for (int i = 0; i < alphavariety; i++) {
        result[i].ch = 0;
        result[i].freq = 0;
    }
    fseek(toread, sizeof(filehead), SEEK_SET);
    fread(result, sizeof(alphafreq), alphavariety, toread);
    fclose(toread);
    return result;
}
void huf_decode(char* source, char* des) {
    filehead info;
    info = readfilehead(source);

    char* origin = "huf";
    for (int i = 0; i < 3; i++) {
        if (origin[i] != info.flag[i]) {
            cout << "错误的文件";
            exit(0);
        }
    }

    alphafreq* alphainfo;
    int alphavariety = 0;
    alphavariety = info.alphaVariety;
    alphainfo = readalphafreq(source,alphavariety,info);
   
   
    
    int hufindex[256] = { 0 }; int lastbit;
    huffman* datahuf;
    datahuf = sethuffmantable(alphainfo, alphavariety, hufindex);
    createhuffmantree(datahuf, alphavariety, 2 * alphavariety-1);
    createhuffmancode(datahuf, alphavariety);
    showmyhuff(datahuf, alphavariety);

    //finished creating huffmancode
    int root = 2 * alphavariety - 2;//start point
    bool finish = false;
    FILE* fzip, * forigin;
    fzip = fopen(source, "rb");
    forigin = fopen(des, "wb");
    

    long filesize, currentplace;
    fseek(fzip, 0, SEEK_END);
    filesize =ftell( fzip);
    fseek(fzip, sizeof(filehead) + alphavariety * sizeof(alphafreq), SEEK_SET);
    currentplace = ftell(fzip);



    //deal with huffmancode in zip
    unsigned char value;
    unsigned char outValue;
    int index = 0;
    int writtehlenth = 0;
    fread(&value, sizeof(unsigned char), 1, fzip);
    while (!finish) {

        if (datahuf[root].lchild == -1 && datahuf[root].rchild == -1) {
            outValue = datahuf[root].data;
            fwrite(&outValue, sizeof(unsigned char), 1, forigin); 
            root = 2 * alphavariety - 2;
            writtehlenth++;
            if (writtehlenth == info.filelenth) {
                break;
            }
        }
        //find the hufcode
        
        if (value & (1 << (7 - index)) ){
            root = datahuf[root].rchild;
        }
        else {
            root = datahuf[root].lchild;
        }
        if (++index >= 8) {
            index = 0;
            value = 0;
            fread(&value, sizeof(unsigned char), 1, fzip);
            currentplace = ftell(fzip);
        }

    }

    fclose(fzip);
    fclose(forigin);
    delete datahuf;

}
bool checkfile(char* source) {
    FILE* target;
    target = fopen(source, "rb");
    bool res;
    res = (target == 0) ? 0 : 1;//0 no exist  1 exist
    if (res != 0) {
        fclose(target);
    }
    return res;

}
void clearstr(char* str) {
    for (int i = 0; i < 50; i++) {
        str[i] = '\0';

    }
}