#include <stdio.h>
#include <stdlib.h>

#define max(a,b) a>b?a:b

struct node{
    // ���� �̸�, �α���, ���� ���� ��
    int name, people, towns;
    // ���� ���� �迭. ũ�Ⱑ ũ�Ƿ� ���� �迭�� ���
    int *adj;
};

/* ���� ����Ʈ */
struct node root[10002];

/* post-order traversal�� �̿��Ͽ� Ž���� ��
�ش� ������ ��� ������ �������� ���� �ִ� �α� ���� �����ϴ� �迭 */
int a[10002];
/* post-order traversal�� �̿��Ͽ� Ž���� ��
�ش� ������ ��� ������ �������� �ʾ��� ���� �ִ� �α� ���� �����ϴ� �迭 */
int b[10002];
/* post-order traversal�� �̿��Ͽ� Ž���� ��
�ش� ������ �̹� �湮�� �������� �ƴ��� üũ */
int flag[10002];

void postOrder(struct node r)
{
    int i;
    /* �湮���� ���� ������ ��쿡 ���Ͽ� Ž�� */
    if(flag[r.name] == 0)
    {
        /* �湮 üũ */
        flag[r.name] = 1;
        
        /* ���� ������ Ž�� */
        for(i=0; i<r.towns; i++)
            postOrder(root[r.adj[i]]);
        
        /* �迭 b�� a�� ���� �Ҵ� */
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
    
    /* ���� ���� �Է� */
    scanf("%d", &n);
    
    /* �ʱ�ȭ(���� ���� �Է�) ~ */
    
    /* 0��° �迭�� ������� �����Ƿ� �ʱ�ȭ */
    root[0].name = 0;
    root[0].towns = 0;
    root[0].people = 0;
    
    /* �Է°��� ���߱� ���� �迭�� 1���� ���� */
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
    /* ~�ʱ�ȭ */
    
    /* Ž�� �������� root�迭�� 1�� ������ ���� ���� */
    postOrder(root[1]);
    
    printf("%d", max(a[1], b[1]));
    
    /* ����� �迭 free */
    for(i=1; i<=n; i++)
       free(root[i].adj);
    
    return 0;
}