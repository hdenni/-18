#include <stdio.h>
#include <stdlib.h>

#define max(a,b) a>b?a:b

struct node{
    // 마을 이름, 인구수, 인접 마을 수
    int name, people, towns;
    // 인접 마을 배열. 크기가 크므로 동적 배열을 사용
    int *adj;
};

/* 마을 리스트 */
struct node root[10002];

/* post-order traversal을 이용하여 탐색할 때
해당 마을을 우수 마을로 선정했을 때의 최대 인구 수를 저장하는 배열 */
int a[10002];
/* post-order traversal을 이용하여 탐색할 때
해당 마을을 우수 마을로 선정하지 않았을 때의 최대 인구 수를 저장하는 배열 */
int b[10002];
/* post-order traversal을 이용하여 탐색할 때
해당 마을이 이미 방문한 마을인지 아닌지 체크 */
int flag[10002];

void postOrder(struct node r)
{
    int i;
    /* 방문하지 않은 마을일 경우에 한하여 탐색 */
    if(flag[r.name] == 0)
    {
        /* 방문 체크 */
        flag[r.name] = 1;
        
        /* 인접 마을을 탐색 */
        for(i=0; i<r.towns; i++)
            postOrder(root[r.adj[i]]);
        
        /* 배열 b와 a의 값을 할당 */
        for(i=0; i<r.towns; i++)
        {
            b[r.name] += max(a[r.adj[i]], b[r.adj[i]]);
            a[r.name] += b[r.adj[i]];
        }
        
        a[r.name] += r.people;
    }
}

int main()
{
    int n;
    int i;
    int x, y;
    
    /* 마을 개수 입력 */
    scanf("%d", &n);
    
    /* 초기화(마을 정보 입력) ~ */
    
    /* 0번째 배열은 사용하지 않으므로 초기화 */
    root[0].name = 0;
    root[0].towns = 0;
    root[0].people = 0;
    
    /* 입력값과 맞추기 위해 배열은 1부터 시작 */
    for(i = 1; i <= n; i++)
    {
        scanf("%d", &root[i].people);
        root[i].name = i;
        root[i].towns = 0;
        root[i].adj = (int*)malloc(sizeof(int));
    }
    for (i = 0 ; i < 10002 ; i++)
        a[i] = b[i] = flag[i] = 0;
    
    for(i = 1; i <= n-1; i++)
    {
        scanf("%d %d", &x, &y);
        root[x].adj[root[x].towns++] = y;
        root[y].adj[root[y].towns++] = x;
        root[x].adj = (int*)realloc(root[x].adj, sizeof(int)*(root[x].towns+1));
        root[y].adj = (int*)realloc(root[y].adj, sizeof(int)*(root[y].towns+1));
    }
    /* ~초기화 */
    
    /* 탐색 시작점은 root배열의 1번 마을로 임의 지정 */
    postOrder(root[1]);
    
    printf("%d", max(a[1], b[1]));
    
    /* 사용한 배열 free */
    for(i=1; i<=n; i++)
       free(root[i].adj);
    
    return 0;
}