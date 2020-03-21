#include <iostream>
#include <list> 
#include <iterator>
#include <algorithm>

/*
1. ask for main plank size
2. ask for number of items to cut
3. repeapt for all ( ask for size, ask for required number)
*/

using namespace std;

struct plank{
    double w,h;
};

struct rect{
    double w, h, area;
     bool operator==(const rect& b) {
        if(h == b.h && w == b.w )
            return true;
        else
            return false;
    }

};

bool compare( rect& a, rect&b ){
    return a.area > b.area;
}

plank plank_inp(){
    plank ptemp;
    cout << "Add meg a lap szeleseget: ";
    cin >> ptemp.w;
    cout << "Add meg a lap magassagat: ";
    cin >> ptemp.h;
    return ptemp;
}

list<rect> rect_inp(int n){
    list<rect> rectlist;
    for (int i=0; i<n; i++){
            double th, tw;
            cout << "Add meg az elem magassagat: ";
            cin >> th;
            cout << "Add meg a lap szelesseget: ";
            cin >> tw;
            double tarea=th*tw;
            rectlist.push_front({tw,th,tarea});
        }
        rectlist.sort(compare);
        return rectlist;
}

void cutdone(list<rect>& rectlist,rect itr){
   rectlist.remove(itr);
}

void cut(rect* r, plank* p, list<plank>& plist){
    double w = (p->w-r->w);
    double a1 = w*p->h;
    double h = (p->h-r->h);
    double a2 = h*r->w;

    plist.pop_front();
    if(w==0 && h==0){
        plist.clear();
    }
    else
    {
        if(w==0){
            plist.push_front({r->w,h});
        }else if(h==0){
            plist.push_front({w,p->h});
        }else{
            if (a1>a2){
                plist.push_front({w,p->h});
                plist.push_front({r->w,h});
            }
            else {
                plist.push_front({r->w,h});
                plist.push_front({w,p->h});
            }
        }
    }   
}

bool cutfits(rect* r, plank* p){
    if ((r->w <= p->w) & (r->h <= p->h))
        return true;
    else
        return false;
}
/// rect to plank
bool cuttingselect(int n, list<rect>& rectlist, plank p, list<plank> &plist){
    list<rect>::iterator itr;
    itr = rectlist.begin();
    int i = 0;
    while ( !cutfits(&*itr,&p) && i<n){
        i++;
        itr++;
    }
    if(i>=n)
        return false;
    else{
        cut(&*itr,&p,plist);
        cutdone(rectlist,*itr);
        return true;
    } 
}
/// plank to rect
bool cuttingselect(int n, list<rect>& rectlist, list<plank> &plist){
    list<plank>::iterator itr;
    itr = plist.begin();
    int i = 0;
    while ( !cutfits(&rectlist.front(),&*itr) && i<n){
        i++;
        itr++;
    }
    if(i>=n)
        return false;
    else{
        cut(&rectlist.front(),&*itr,plist);
        cutdone(rectlist, rectlist.front());
        return true;
    } 
}

int main()
{
    list<plank> p;
    list<plank> waste;
    plank ptemp = plank_inp();
    p.push_back(ptemp);
    int n;
    cout << "Add meg a vagando elemek szamat: ";
    cin >> n;
    list<rect> rectlist = rect_inp(n);

    while(!p.empty() && !rectlist.empty()){
        if(!cuttingselect(n, rectlist,p.front(), p)){
            waste.push_front(p.front());
            p.pop_front();
        }
    }
    while(!p.empty()){
        if(rectlist.empty()){
            waste.push_front(p.front());
            p.pop_front(); 
        }
    }
    cout << waste.empty();        
}
