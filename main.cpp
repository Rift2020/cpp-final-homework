#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>
using namespace std;
class book{
protected:
    string bookID;
    string bookName;
    string author;
    string publishing;
    double price;
public:
    book();
    book(const string &id,const string &nam,const string &aut,const string &pub,double pri);
    string getBookID();
    string getBookName();
    string getAuthor();
    string getPublishing();
    double getPrice();
    friend ostream &operator<<(ostream &output,const book &d);
    friend bool compareByName(book a,book b);//С�ڵıȽ�
    void display();
};
class bookList{
protected:
    vector<book> list;
    string sortedMode;
public:
    book getBook(int index){
        //����list�����ж�Ӧ�������鱾
        return list[index];
    }
    void initBookList(const char *filename){
        //�����ļ��еĲ˵�����װ��list
        ifstream inFile;
        inFile.open("bookList.txt",ios::in);
        char id[80],nam[80],aut[80],pub[80];
        double pri;
        while (inFile>>id){
            inFile.ignore();
            inFile.getline(nam,80);
            inFile.getline(aut,80);
            inFile.getline(pub,80);
            inFile>>pri;
            list.emplace_back(book(id,nam,aut,pub,pri));
        }
        inFile.close();
    }
    void display(){
        int j=0;
        cout<<"bookSize:"<<list.size()<<endl;
        for(const auto &i:list)
            cout<<j++<<" "<<i<<endl;
    }
    void sortByName();
    int findByName(string fName);//����к�fNameһ������������������list�е����������򷵻�-1������)
};
bookList bl;//ȫ���鵥����
class buyer{
protected:
    string name;
    int buyerID;
    string address;
    double pay;
    map<int,int> boughtList;//key�����������������bookList�е�������value���������������
public:
    buyer();
    buyer(const string &n,int b,const string &a,double p);
    string getBuyName();
    string getAddress();
    double getPay();
    int getBuyerId();
    virtual void displayBoughtList();
    void addBook(int index);//����һ����
    void clearList();
    bool changeBook(int index,int num);//�ı�����Ϊindex������,����ֵΪ��˵��numΪ�����Ƿ���Ϊ��
    friend ostream &operator<<( ostream &output,const buyer &D );
    virtual double setPay();//�����ܼ۲�װ��pay������ֵΪ�ܼ�
};
class member:public buyer{
protected:
    int grade;//ÿ����Ա����5%�ķ���
public:
    member();
    member(const string &n,int b,int g,const string &a,double p);
    int getGrade();
    friend ostream &operator<<( ostream &output,const member &d );
    double setPay();
    void storeBoughtList();
};
member opMember("unLogin",0,0,"NULL",0);//ȫ�ֵ�½�û�����
class memberList{
protected:
    vector<member> list;
public:
    void initMemberList(){
        ifstream inFile;
        inFile.open("memberList.txt",ios::in);
        string nam,add;
        int id,g;
        while(inFile>>nam){
            inFile>>id>>g>>add;
            list.emplace_back(member(nam,id,g,add,0));
        }
        inFile.close();
        for(auto &i:list){
            string nam=i.getBuyName()+".txt";
            ifstream inFile;
            inFile.open(nam,ios::in);
            if(!inFile.is_open())continue;
            char bookName[80];
            int amount;
            while(inFile.getline(bookName,80)){
                inFile>>amount;
                inFile.ignore();
                int index=bl.findByName(bookName);
                if(index==-1)
                    cout<<bookName<<" is sold out"<<endl;
                else
                    i.changeBook(index,amount);
            }
            i.setPay();
            inFile.close();
        }
    }
    void display(){
        for(auto i:list){
            cout<<i<<endl;
        }
    }
    void addMember(member m){
        list.emplace_back(m);
        ofstream inFile;
        inFile.open("memberList.txt",ios::app);
        inFile<<m.getBuyName()<<" "<<m.getBuyerId()<<" "<<m.getGrade()<<" "<<m.getAddress()<<endl;
    }
    int findByName(string nam){//�������������û���Ϊnam���û����ҷ���ml�е�������û�з���-1
        for(auto i=list.begin();i!=list.end();++i){
            if(i->getBuyName()==nam)return i-list.begin();
        }
        return -1;
    }
    member& getMemberReference(int index){
        //����list�е�member����
        if(index<0||index>=list.size()){
            cout<<"illegal index"<<endl;
            exit(0);
        }
        return list[index];
    }
};
memberList ml;//ȫ�ֻ�Ա�� ����
class interface{
public:
    virtual void show()=0;
};
class theInterface{
public:
    static interface *in;
    static void run(){
        while(1){
            in->show();
        }
    };
};
class login;
class loginMenu:public interface{
public:
    void show();
};
class login:public interface{
public:
    void show();
};
class registe:public interface{
public:
    void show();
};
class storeMenu:public interface{
public:
    void show();
};
class buy:public interface{
public:
    void show();
};
class findBook:public interface{
public:
    void show();
};
void loginMenu::show(){
    cout<<"welcome,this is the onlineBookstore"<<endl;
    cout<<"login(1) or register(2) or guest(3)"<<endl;
    int op;
    cin>>op;
    if(op==1)
        theInterface::in=new login();
    else if(op==2)
        theInterface::in=new registe();
    else if(op==3)
        theInterface::in=new storeMenu();
}
void login::show() {
    cout<<"login:"<<endl;
    cout<<"input your name or input \"back\":"<<endl;
    string op;
    cin>>op;
    if(op=="back")
        theInterface::in=new loginMenu();
    else
        if(ml.findByName(op)==-1) {
            cout << "username has not registered" << endl;
            theInterface::in = new login();
        }
        else{
            cout<<"login success"<<endl;
            opMember=ml.getMemberReference(ml.findByName(op));
            theInterface::in=new storeMenu();
        }
}
void registe::show() {
    cout<<"register:"<<endl;
    cout<<"input your name,id,grade,address"<<endl;
    string nam,add;
    int gra,id;
    cin>>nam>>id>>gra>>add;
    if(nam=="back"||bl.findByName(nam)!=-1){
        cout<<"illegal name or name has been used,retry!"<<endl;
        theInterface::in=new registe();
    }
    opMember=member(nam,id,gra,add,0);
    ml.addMember(opMember);
    cout<<"register success"<<endl;
    theInterface::in=new storeMenu();
}
void storeMenu::show() {
    cout<<"welcome to buy some book!username:"<<opMember.getBuyName()<<endl;
    cout<<"1.Display all book"<<endl;
    cout<<"2.find a book"<<endl;
    cout<<"3.store my shopping cart"<<endl;
    cout<<"4.settle accounts"<<endl;
    cout<<"5.log out"<<endl;
    int op;
    cin>>op;
    if(op==1){
        bl.display();
        theInterface::in=new buy();
    }
    else if(op==2){
        theInterface::in=new findBook();
    }
    else if(op==3){
        if(opMember.getBuyName()=="unLogin"){
            cout<<"you have not login"<<endl;
        }
        else{
            opMember.storeBoughtList();
            cout<<"store success"<<endl;
        }
    }
    else if(op==4){
        opMember.displayBoughtList();
        cout<<"total price:"<<opMember.setPay()<<endl;
        opMember.clearList();
    }
    else if(op==5){
        opMember=member("unLogin",0,0,"NULL",0);//�ǳ��򽫿����û�����δδ��¼
        theInterface::in=new loginMenu();
    }
}
void buy::show(){
    fflush(stdin);
    stringstream ss;
    ss.clear();
    char inp[80];
    cout<<"input one number to buy just one book,input two number to buy many books or change amount,input \"cart\" to display your cart,input \"back\" to back"<<endl;
    cin.getline(inp,80);
    string input=inp;
    //�ж��ǲ���ָ�������ǣ����˵�stream���в��Ұ���������������
    if(input=="cart"){
        opMember.displayBoughtList();
    }
    else if(input=="back"){
        theInterface::in=new storeMenu();        
    }
    else{
        ss<<input;
        int index,amount;
        ss>>index;
        if(ss>>amount){
            opMember.changeBook(index,amount);
        }else{
            opMember.addBook(index);
        }
    }
    //theInterface::in=new buy();
    
}
void findBook::show() {
    cout<<"input whole topic to find a book,input \"back\" to back"<<endl;
    string nam;
    cin>>nam;
    if(nam=="back")theInterface::in=new storeMenu();
    int index=bl.findByName(nam);
    if(index==-1){
        cout<<"no such book"<<endl;
    }
    else{
        cout<<index<<" "<<bl.getBook(index);
    }
}
member::member() {
    grade=0;
}
member::member(const string &n,int b,int g,const string &a,double p): buyer(n,b,a,p){
    grade=g;
}
double member::setPay() {
    double sp=0;
    for(auto i:boughtList){
        sp+=bl.getBook(i.first).getPrice()*i.second;
    }
    sp*=(1-0.05*grade);
    pay=sp;
    return sp;
}
ostream &operator<<( ostream &output,const member &d ){
    output<<"name:"<<d.name<<endl;
    output<<"grade:"<<d.grade<<endl;
    output<<"buyerID:"<<d.buyerID<<endl;
    output<<"address:"<<d.address<<endl;
    return output;
}
int member::getGrade(){
    return grade;
}
void buyer::clearList() {
    boughtList.clear();
}
void member::storeBoughtList() {
    ofstream outFile;
    outFile.open(name+".txt",ios::trunc);
    for(auto i:boughtList){
        outFile<<bl.getBook(i.first).getBookName()<<endl;
        outFile<<i.second<<endl;
    }
    outFile.close();
}
book::book() {}
book::book(const string &id,const string &nam,const string &aut,const string &pub,double pri) {
    bookID=id,bookName=nam,author=aut,publishing=pub,price=pri;
}
string book::getBookID() {
    return bookID;
}
string book::getBookName() {
    return bookName;
}
string book::getAuthor() {
    return author;
}
string book::getPublishing() {
    return publishing;
}
double book::getPrice() {
    return price;
}
bool compareByName(book a,book b){
    if(a.getBookName()<b.getBookName())return true;
    return false;
}
ostream &operator<<(ostream &output,const book &d){
    output<<"bookID:"<<d.bookID<<endl;
    output<<"bookName:"<<d.bookName<<endl;
    output<<"author:"<<d.author<<endl;
    output<<"publishing:"<<d.publishing<<endl;
    output<<"price:"<<d.price<<endl;
    return output;
}
buyer::buyer() {pay=0;}
buyer::buyer(const string &n, int b, const string &a, double p) {
    name=n,buyerID=b,address=a,pay=p;
}
string buyer::getBuyName() {
    return name;
}
string buyer::getAddress() {
    return address;
}
double buyer::getPay() {
    return pay;
}
int buyer::getBuyerId() {
    return buyerID;
}
void buyer::displayBoughtList() {
    for(auto i:boughtList){
        cout<<bl.getBook(i.first).getBookName()<<" "<<i.second<<endl;
    }
}
double buyer::setPay(){
    double sp=0;
    for(auto i:boughtList){
        sp+=bl.getBook(i.first).getPrice()*i.second;
    }
    pay=sp;
    return sp;
}
ostream &operator<<( ostream &output,const buyer &d ){
    output<<"name:"<<d.name<<endl;
    output<<"buyerID:"<<d.buyerID<<endl;
    output<<"address:"<<d.address<<endl;
    return output;
}
void buyer::addBook(int index) {
    if(index<0)return;
    if(boughtList.find(index)!=boughtList.end()){//������м�ֵ��
        boughtList[index]++;
    }else{
        boughtList[index]=1;
    }
}
bool buyer::changeBook(int index,int num){
    if(index<0)return false;
    if(num<0)return false;
    if(num==0){
        //�������������Ϊ0����ô���������ֵ��
        boughtList.erase(index);
    }
    else{
        boughtList[index]=num;
    }
    return true;
}

void bookList::sortByName(){
    sort(list.begin(),list.end(), compareByName);
    sortedMode="name";
}
int bookList::findByName(string fName) {
    if(sortedMode!="name")
        sortByName();
    int l=0,r=list.size()-1,m=(l+r)/2;
    while(l<r){
        //����
        if(list[l].getBookName()==fName)
            return l;
        if(list[r].getBookName()==fName)
            return r;
        if(l==r-1)return -1;
        if(list[m].getBookName()<fName)
            l=m;
        else r=m;
        m=(l+r)/2;
    }
}
interface* theInterface::in=new loginMenu();//ȫ�ֽ���ָ�룬����ָ�����ǰ����ʾ��ҳ��
int main() {
    //blΪȫ���鵥�������mlΪȫ�ֻ�Ա�����
    bl.initBookList("bookList.txt");
    ml.initMemberList();
    theInterface::run();//������ʾ��runΪ��ѭ����Ա����
    return 0;
}
