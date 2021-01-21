#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#pragma comment(linker,"/STACK:1024000000,1024000000")
#define abs(a) ((a)>0?(a):(-(a)))
#define lowbit(a) (a&(-a))
#define sqr(a) ((a)*(a))
#define mem(a,b) memset(a,b,sizeof(a))
typedef long long LL;
typedef long long TypePoint;
typedef long long TypeYear;
typedef long long TypeDist;
const long long INF = 1LL * INT_MAX * 1e8;
const int LIMIT = 100000000;
using namespace std;

class Point
{
public:
    TypePoint x, y;
    TypeYear year;

    Point() {}
    Point(TypePoint x, TypePoint y, TypeYear year) : x(x), y(y), year(year) {}

    bool operator==(const Point &o) const
    {
        return (x == o.x) && (y == o.y) && (year == o.year);
    }
    bool operator!=(const Point &o) const
    {
        return (x != o.x) || (y != o.y) || (year != o.year);
    }
    bool operator<(const Point &o) const
    {
        if (year != o.year)
            return (year < o.year);
        if (x != o.x)
            return (x < o.x);
        return (y < o.y);
    }
    bool operator>(const Point &o) const
    {
        if (year != o.year)
            return (year > o.year);
        if (x != o.x)
            return (x > o.x);
        return (y > o.y);
    }
    TypeDist operator-(const Point &o) const
    {
        return (year != o.year) + max(abs(x - o.x), abs(y - o.y));
    }
    TypeDist operator+(const Point &o) const
    {
        return abs(year - o.year) + 10LL * max(abs(x - o.x), abs(y - o.y))
               +  4 * min(abs(x - o.x), abs(y - o.y));
    }
};

LL n;
string method;
TypePoint W, H;
Point S, T;
set<Point> P;
map<TypeYear, set<Point>> Y;
map<Point, set<Point>> C;
map<Point, TypeDist> guess;
map<Point, TypeDist> dis;
map<Point, Point> fa;

class Pqueue
{
public:
    TypeDist cost;
    Point from;
    Pqueue() {}
    Pqueue(TypeDist cost, Point from): cost(cost), from(from) {}
    bool operator < (const Pqueue& o) const
    {
        if (cost + guess[from] != o.cost + guess[o.from])
            return cost + guess[from] > o.cost + guess[o.from]; // small
        return (from > o.from);
    }
};

inline TypeDist getDist(Point aa, Point bb)
{
    return (method == "BFS") ? (aa - bb) : (aa + bb);
}

inline bool within(Point aa)
{
    return (0 <= aa.x && aa.x < W && 0 <= aa.y && aa.y < H);
}

void input()
{
    LL i, j, k;
    TypePoint x, y, z;
    TypeYear by, ey;
    cin >> method;
    cin >> W >> H;
    cin >> S.year >> S.x >> S.y;
    cin >> T.year >> T.x >> T.y;
    cin >> n;
    swap(S, T);
    for (i = 1; i <= n; i++)
    {
        //cin >> by >> x >> y >> ey;
        scanf("%lld%lld%lld%lld", &by, &x, &y, &ey);
        if (by == ey)
        {
            continue;
        }
        if (by > ey)
        {
            swap(by, ey);
        }
        Point aa = Point(x, y, by);
        Point bb = Point(x, y, ey);
        P.insert(aa);
        P.insert(bb);
        C[aa].insert(bb);
        C[bb].insert(aa);
    }
    P.insert(S);
    P.insert(T);
    if (n > LIMIT)
        return;
    for (auto p : P)
    {
        Y[p.year].insert(p);
        guess[p] = getDist(p, T);
    }
}

TypeDist BFS()
{
    int dx[4] = {-1, -1, 1, 1};
    int dy[4] = {-1, 1, -1, 1};
    int dxx[4] = {-1, 0, 0, 1};
    int dyy[4] = {0, -1, 1, 0};
    TypeDist mini = INF;
    priority_queue<Pqueue> q;
    q.push(Pqueue(0,S));
    dis[T] = INF;
    dis[S] = 0;
    while(!q.empty())
    {
        Pqueue now = q.top();
        q.pop();
        if (now.from == T)
            break;
        if (now.cost > dis[now.from])
            continue;
        if (now.cost + getDist(now.from, T) >= mini)
            break;
        for (auto to : C[now.from])
        {
            TypeDist z = getDist(now.from, to);
            if (dis.find(to) == dis.end() || now.cost + z < dis[to])
            {
                dis[to] = now.cost + z;
                q.push(Pqueue(dis[to], to));
                fa[to] = now.from;
                if (to == T)
                {
                    mini = min(mini, dis[T]);
                }
            }
        }
        for (int k = 0; k < 4; k++)
        {
            Point to(now.from.x + dx[k], now.from.y + dy[k], now.from.year);
            if (!within(to))
                continue;
            TypeDist z = getDist(now.from, to);
            if (dis.find(to) == dis.end() || now.cost + z < dis[to])
            {
                dis[to] = now.cost + z;
                q.push(Pqueue(dis[to], to));
                fa[to] = now.from;
                if (to == T)
                {
                    mini = min(mini, dis[T]);
                }
            }
        }
        for (int k = 0; k < 4; k++)
        {
            Point to(now.from.x + dxx[k], now.from.y + dyy[k], now.from.year);
            if (!within(to))
                continue;
            TypeDist z = getDist(now.from, to);
            if (dis.find(to) == dis.end() || now.cost + z < dis[to])
            {
                dis[to] = now.cost + z;
                q.push(Pqueue(dis[to], to));
                fa[to] = now.from;
                if (to == T)
                {
                    mini = min(mini, dis[T]);
                }
            }
        }
    }
    return dis[T];
}

TypeDist SPFA()
{
    TypeDist mini = INF;
    for (auto p : P)
    {
        dis[p] = INF;
    }
    map<Point, bool> mark;
    queue<Point> q;
    q.push(S);
    dis[S] = 0;
    mark[S] = 1;
    while (!q.empty())
    {
        Point now = q.front();
        q.pop();
        mark[now] = 0;
        if (now == T || dis[now] + guess[now] > mini)
        {
            continue;
        }
        for(auto to : Y[now.year])
        {
            TypeDist z = getDist(now, to);
            if (dis[to] > dis[now] + z)
            {
                dis[to] = dis[now] + z;
                fa[to] = now;
                if (mark.find(to) == mark.end() || !mark[to])
                {
                    mark[to] = 1;
                    q.push(to);
                    if (to == T)
                    {
                        mini = min(mini, dis[T]);
                    }
                }
            }
        }
        for(auto to : C[now])
        {
            TypeDist z = getDist(now, to);
            if (dis[to] > dis[now] + z)
            {
                dis[to] = dis[now] + z;
                fa[to] = now;
                if (mark.find(to) == mark.end() || !mark[to])
                {
                    mark[to] = 1;
                    q.push(to);
                    if (to == T)
                    {
                        mini = min(mini, dis[T]);
                    }
                }
            }
        }
    }
    return dis[T];
}

TypeDist Dijkstra()
{
    //map<Point, bool> mark;
    TypeDist mini = INF;
    for (auto p : P)
    {
        dis[p] = INF;
    }
    dis[S] = 0;
    priority_queue<Pqueue> q;
    q.push(Pqueue(0, S));
    while (!q.empty())
    {
        Pqueue now = q.top();
        q.pop();
        //mark[now.from] = 1;
        if (now.from == T)
            break;
        if (now.cost > dis[now.from])
            continue;
        if (now.cost + guess[now.from] > mini)
            break;
        for (auto to : Y[now.from.year])
        {
            TypeDist z = getDist(now.from, to);
            if (now.cost + z < dis[to])
            {
                dis[to] = now.cost + z;
                q.push(Pqueue(dis[to], to));
                fa[to] = now.from;
                if (to == T)
                {
                    mini = min(mini, dis[T]);
                }
            }
        }
        for (auto to : C[now.from])
        {
            TypeDist z = getDist(now.from, to);
            if (now.cost + z < dis[to])
            {
                dis[to] = now.cost + z;
                q.push(Pqueue(dis[to], to));
                fa[to] = now.from;
                if (to == T)
                {
                    mini = min(mini, dis[T]);
                }
            }
        }
    }
    return dis[T];
}

void outPath(Point aa, Point bb)
{
    if (aa.year != bb.year)
    {
        //cout << bb.year << " " << bb.x << " " << bb.y << " " << getDist(aa, bb) << endl;
        printf("%lld %lld %lld %lld\n", bb.year, bb.x, bb.y, getDist(aa, bb));
        return;
    }
    TypePoint x = aa.x;
    TypePoint y = aa.y;
    TypeYear z = aa.year;
    TypePoint dx = bb.x - aa.x;
    TypePoint dy = bb.y - aa.y;
    dx = dx == 0 ? 0 : dx > 0 ? 1 : -1;
    dy = dy == 0 ? 0 : dy > 0 ? 1 : -1;
    int dz = (method == "BFS") ? 1 : 14;
    while (x != bb.x && y != bb.y)
    {
        x += dx;
        y += dy;
        //cout << z << " " << x << " " << y << " " << dz << endl;
        printf("%lld %lld %lld %d\n", z, x, y, dz);
    }
    if (x == bb.x)
        dx = 0;
    if (y == bb.y)
        dy = 0;
    dz = (method == "BFS") ? 1 : 10;
    while (x != bb.x || y != bb.y)
    {
        x += dx;
        y += dy;
        //cout << z << " " << x << " " << y << " " << dz << endl;
        printf("%lld %lld %lld %d\n", z, x, y, dz);
    }
}

void output(TypeDist ans)
{
    if (ans == INF)
    {
        //cout << "FAIL" << endl;
        puts("FAIL");
        return;
    }
    //cout << ans << endl;
    printf("%lld\n", ans);
    Point aa, bb;
    TypeDist step = 1;
    for (aa = T, bb = fa[aa]; aa != S; bb = fa[aa = bb])
    {
        step += (aa.year != bb.year) ? 1 : max(abs(aa.x - bb.x), abs(aa.y - bb.y));
    }
    //cout<< step << endl;
    printf("%lld\n", step);
    //cout << T.year << " " << T.x << " " << T.y << " " << 0 << endl;
    printf("%lld %lld %lld 0\n", T.year, T.x, T.y);
    for (aa = T, bb = fa[aa]; aa != S; bb = fa[aa = bb])
    {
        outPath(aa, bb);
    }
}

int main()
{
    freopen ("input.txt", "r", stdin);
    freopen ("output.txt", "w", stdout);
    TypeDist ans;
    input();
    if (n > LIMIT)
    {
        ans = BFS();
    }
    else
    {
        //ans = SPFA();
        ans = Dijkstra();
    }
    output(ans);
    return 0;
}
