#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QFile>
#include "libtabfile.h"
#include "qdebug.h"

#define MAX_STR_LEN 4000

struct _worditem
{
    char *word;
    char *definition;
};

static int comparefunc(const void *a, const void *b){
    int x;
    //a.word가 b.word보다 크면 +, a.word가 b.word보다 작으면 -, a.word가 b.word와 같으면 0을 반환한다
    //x = strcmp(((**(struct _worditem **)a).word), ((**(struct _worditem **)b).word));
    x = strcmp((**(struct _worditem **)a).word, (**(struct _worditem **)b).word);
    printf("a: %d\n", x);
    //만약 반환값이 0이면, 즉 a.word와 b.word가 같으면
    //그것이 가리키는 definition주소값이 같은지, 즉 같은 definition을 가리키고 있는지 검사한다.
    if(x==0){
        //definition의 주소가 다르다면 a와 b는 다른 데이터이므로 +나 -를 반환하고
        //definition의 주소가 같다면 a와 b는 정확히 같은 데이터이므로 0을 반환한다.
        return ((struct _worditem*)a)->definition - ((struct _worditem*)b)->definition;
    }else{
        return x;
    }
}

//앞뒤 공백문자 제거함수
int trim (char *strOld)
{
        int len = 0;
        char strNew[MAX_STR_LEN];
        int offset = 0;
        int i;

        len = strlen(strOld);
        if (len >= MAX_STR_LEN)
        {
                puts ("string too long");
                return -1;
        }

        strcpy(strNew, strOld);

        // 앞에거 잘라내기
        for (i = 0; i < len; i++)
        {
                if (strNew[i] == ' ' || strNew[i] == '\t')
                        offset++;
                else
                        break;
        }

        // 뒤에거 잘라내기
        for (i = len-2; i >= 0; i--)
        {
                if (strNew[i] == ' ' || strNew[i] == '\t' || strNew[i] == '\n')
                        strNew[i] = '\0';
                else
                        break;
        }

        strcpy (strOld, strNew+offset);

        return strlen(strOld);
}

static bool read_tab_file(char *buffer, struct _worditem *worditems[]){
    char *p, *p1, *p2; //p는 현재 읽고 있는 내용, p1은 읽을 위치
    p = buffer;

    int linenum = 0;

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
        memcpy(worditems[linenum],&worditem,sizeof(struct _worditem)); //worditem의 내용을 _worditem의 길이만큼 읽음

        //        바로 위 3줄과 같은 내용이다.
        //        worditems[i]->word = p;
        //        worditems[i]->definition = p2;

        p = p1;
        linenum++;
    }
    return true;
}

void convert_tabfile(const char *filename){
    QFile file;
    file.setFileName(filename);

    struct _worditem *array[5];
    for(int i = 0; i<sizeof(array)/sizeof(struct _worditem *); i++){
        array[i] = (struct _worditem *)malloc(sizeof(struct _worditem));
        memset(array[i],0,sizeof(struct _worditem));
    }


    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        char *buf = file.readAll().data();
        read_tab_file(buf, array);
        file.close();

        //qsort(array,100,sizeof(_worditem *),comparefunc);
        qsort(array,sizeof(array)/sizeof(_worditem *),sizeof(_worditem *),comparefunc);
        for(int i=0; i<sizeof(array)/sizeof(struct _worditem *);i++){
            printf("item[%d].word: %s\n", i, array[i]->word);
            printf("item[%d].definition: %s\n", i, array[i]->definition);
        }

    }

    for(int i = 0; i<sizeof(array)/sizeof(struct _worditem *); i++){
        free(array[i]);
    }
}
