#include <iostream>
#include <list> 
#include <iterator>
#include <algorithm>
using namespace std;

/*
1. ask for main plank size
2. ask for number of items to cut
3. repeapt for all ( ask for size, ask for required number)
*/

#define CW 1.0 // the with of the waste caused by the cutting blade
#define WS 5.0 // defines the minimal dimension of plank not considered waste

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
bool operator==(plank a, const plank& b) {  // nem tudom miert kell de enelkul errort dob
        if(a.h == b.h && a.w == b.w )
            return true;
        else
            return false;
    }
bool compare( rect& a, rect&b ){
    return a.area > b.area;
}

plank plank_inp(){  // function for inputing the dimension of a plank (temp)
    plank ptemp;
    cout << "Add meg a lap szeleseget: ";
    cin >> ptemp.w;
    cout << "Add meg a lap magassagat: ";
    cin >> ptemp.h;
    return ptemp;
}

list<rect> rect_inp(int n){     // reading in an 'n' number of rectangles to cut and passing them back in a list
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
        rectlist.sort(compare);   //    sorting items in a decreasing order based on the area
        return rectlist;
}

void printImg(rect r, plank p){   // deals with visualizing the necessary cuts to the user (temp)
    cout << "Vertical cut: "<< r.w+(CW/2) << "Horizontal cut: "<< r.h+(CW/2)<< endl;
}

void cutdone(list<rect>& rectlist,rect done, plank p){    // function that contains the necessary operations to be done after cutting out a piece
    rectlist.remove(done);
    printImg(done, p);
}

void cut(rect* r, plank* p, list<plank>& plist){    // function to do the 'cutting' operation, vertical cut first horizontal second
    double w = (p->w-r->w);     // the width of the first new plank
    double a1 = w*p->h;         // the area of the first new plank  [isn't realy need, could be done inline but its easier to read]
    double h = (p->h-r->h);     // the height of the second new plank
    double a2 = h*r->w;         // the area of the second new plank

    // plist.pop_front();       // removes the first element of the list (the one which has just been cut up) [not used anymore]
    plist.remove(*p);           // removes the plank form the list that we just cut up
    if(w==0 && h==0){           // if the plank size = rect size, no new planks needed to be added to the list [needs better solution]
        
    }
    else{   if(w==0){               // if true only one new plank needs to be added (bottom)
            plist.push_front({r->w,h});
        }else if(h==0){             // if true only one new plank needs to be added (right)
            plist.push_front({w,p->h});
        }else{                      // if none of them are true, add 2 new planks to list, smaller first, larger second
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

int cutfits(rect* r, plank* p){ // return 1 if cut fits, -1 if fits but needs rotation and 0 if cut doesn't fit
    if ((r->w <= p->w) & (r->h <= p->h))
        return 1;
    else if ((r->h <= p->w) & (r->w <= p->h))
            return -1;
        else
            return 0;
}

rect rotateRect(rect* r){
    return rect {r->h, r->w};
}
/// rect to plank
bool cuttingselect(int n, list<rect>& rectlist, plank p, list<plank> &plist){
    list<rect>::iterator itr;
    itr = rectlist.begin();
    int i = 0;
    while ( (cutfits(&*itr,&p) != 0 ) && i<n){  // iterate on rectlist until a cut fits or we tested every member
        i++;
        itr++;
    }
    if(i>n)
        return false;
    else{
        if ( cutfits(&*itr,&p) == -1)       // if needs rotation, rotate 90
            *itr = rotateRect(&*itr);
        cut(&*itr,&p,plist);
        cutdone(rectlist,*itr,p);
        return true;
    } 
}
/// plank to rect [not currently in use] (about equivalent time necessity, up to preference) [not tested]
bool cuttingselect(int n, list<rect>& rectlist, list<plank> &plist){
    list<plank>::iterator itr;
    itr = plist.begin();
    int i = 0;
    while ( (cutfits(&rectlist.front(),&*itr) != 0 ) && i<n){
        i++;
        itr++;
    }
    if(i>n)
        return false;
    else{
        if ( cutfits(&rectlist.front(),&*itr) == -1)       // if needs rotation, rotate 90
            rectlist.front() = rotateRect(&rectlist.front());
        cut(&rectlist.front(),&*itr,plist);
        cutdone(rectlist, rectlist.front(), *itr);
        return true;
    } 
}

int main()
{
    list<plank> p;
    list<plank> waste;
    list<plank> aside;      // storage for planks away from p
   // plank ptemp = plank_inp();
    p.push_back(plank_inp());
    int n;
    cout << "Add meg a vagando elemek szamat: ";
    cin >> n;
    list<rect> rectlist = rect_inp(n);

    while(!p.empty() && !rectlist.empty()){     // amig van vágando elem es vághato lap
        /* valamiert a compiler nem szereti 
        for(auto itr: p){        // check for waste, if found add to waste, if one found which isn't break (since the list is in inreasing order)
            if (itr.h < WS || itr.w < WS){
                waste.push_front(itr);
                p.remove(itr);
            }else
                break;
        }*/
/*
        list<plank>::iterator itr;
        itr = p.begin(); 
        while(itr != p.end()){        // check for waste, if found add to waste, if one found which isn't break (since the list is in inreasing order)
            if (itr->h < WS || itr->w < WS){
                waste.push_front(*itr);
                p.remove(*itr);
            }else
                break;
            itr++;
        }
*/
        if(!cuttingselect(n, rectlist,p.front(), p)){
            aside.push_front(p.front());         // if cutting is unsucesfull add p to waste/aside ??? [NEEDS REWORK URGENT] (if cutting is unsuccesful store aside and later add back)
            p.pop_front();
        }
        /*if(!cuttingselect(n, rectlist, p)){   // or use other cuttingselect
            aside.push_front(p.front());
            p.pop_front();
        }*/
    }
    cout << "waste found(bool): "<<waste.empty();        
}
