#include <stdio.h>
#include <string.h>
#include <QFile>
#include "libtabfile.h"

struct _worditem
{
    char *word;
    char *definition;
};

static int comparefunc(const void *a, const void *b){
    int x;
    x = strcmp(((struct _worditem *)a)->word, ((struct _worditem *)b)->word);
    if(x==0){
        return ((struct _worditem*)a)->definition - ((struct _worditem*)b)->definition;
    }else{
        return x;
    }
}

static bool read_tab_file(char *buffer, _worditem *worditems[]){
    char *p, *p1, *p2; //p는 현재 읽고 있는 내용, p1은 읽을 위치
    p = buffer;

    int i = 0;
    struct _worditem worditem;
    while(true){
        if(*p == '\0'){
            printf("Conversion is over.\n");
            break;
        }
        p1 = strchr(p, '\n');   //p가 가리키는 내용중 줄의 끝 표시가 나오는 위치를 p1에 저장한다.
        *p1 = '\0';             //p1이 가리키는 위치의 내용, 즉 p에서 줄의 끝 표시 대신에 문장의 끝 표시를 한다.
        p1++;                   //다음 단어(word,definition의 쌍)의 위치로 p1포인터를 옮겨 놓고,
        p2 = strchr(p,'\t');    //p에서 탭문자(word의 끝)의 위치를 찾는다. 단, 이 때 p는 0으로 끝나는 한 단어이다.
        *p2 = '\0';             //p2가 가리키는 위치의 내용, 즉 p에서 word의 끝 표시 대신에 문장의 끝 표시를 한다.
        p2++;
        worditem.word = p;      //결국 p는 '\0'으로 끝나는 word만 남는다.
        worditem.definition = p2;   //p2위치에서 '\0'으로 끝나기까지는 definition이 된다.
        memcpy(worditems[i],&worditem,sizeof(struct _worditem));
        printf("word(%i): %s\n", i, worditems[i]->word);
        printf("definition(%i): %s\n", i, worditems[i]->definition);
        i++;
        p = p1;
    }
    return true;
}

void convert_tabfile(const char *filename){
    QFile file;
    file.setFileName(filename);
    _worditem *array;
    array = (_worditem *)malloc(sizeof(_worditem)*100);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        char *buf = file.readAll().data();
        read_tab_file(buf, &array);
        file.close();
    }
    free(array);
}
