#include <iostream>
#include <fstream>
#include <dirent.h>
#include <ctime>

using namespace std;
struct BooksNames {
    string BookName;
    BooksNames * Next;
};
struct Dictionary {
        string Words;
        BooksNames *Books;
        int x;
        Dictionary * Next;
};
Dictionary * Dic();
void test(Dictionary *);
int DicCheck(string,Dictionary *,string);
Dictionary *SortWords(Dictionary *,string);
string CleanWords(string);
string CleanBooks(string);
int HLCheckOne(string,string);
void SearchWords(Dictionary*);
void Delete(Dictionary*);
void Folder();
int main(){
    Dictionary *Head;
    clock_t start,end;
    start = clock();

    Folder();
    Head = Dic();

    end = clock();
    double runtime = double(end - start)/CLOCKS_PER_SEC;
    cout<<"Creating Dictionary Time : "<<runtime<<" seconds."<<endl;

    int menu=0;
    while (menu!=-1){
        cout<<"1)Search a Word and show , which Books have the Word.\n2)Test a Word and a Book to say is the Word in that Book or not.\n3)Exit.\n";
        cin>>menu;
        switch (menu)
        {
        case 1 : SearchWords(Head);break;
        case 2 : test(Head);break;
        case 3 : menu = -1;break;
        }
    }
    Delete(Head);
}
Dictionary * Dic(){
    Dictionary *Head=NULL;
    Dictionary *curr;
    Dictionary *pre;
    Dictionary *New;
    string word;
    string Bookname[84];
    ifstream Books("/Users/largexim/Documents/T2AP/T5/Filename.txt");

    for(int i=0 ; i<84 ; i++){
        getline(Books,Bookname[i],'\n');
    }

    for(int i=0 ; i<2 ; i++){
        ifstream Book("/Users/largexim/Documents/T2AP/T5/File/"+Bookname[i]);

        while(Book>>word){
            word = CleanWords(word);
            if(DicCheck(word,Head,Bookname[i])){
                pre = Head;
                curr = SortWords(Head,word);

                if(curr==Head){
                    New = new struct Dictionary;
                    New->Words=word;
                    New->Books = new struct BooksNames;
                    New->Books->BookName = Bookname[i];
                    New->x =1;
                    New->Next = Head;
                    Head = New;
                }
                else{
                    New = new struct Dictionary;
                    New->Words = word;
                    New->Books = new struct BooksNames;
                    New->Books->BookName = Bookname[i];
                    New->x =1;
                    New->Next = curr;
                    while(pre->Next!=curr)
                    pre = pre->Next;
                    pre->Next = New;
                }
            }
        }
    }
        return Head;
}
string CleanWords(string word){
    string cleaned = "";
    for(int i=0 ; word[i] ; i++){
        if(('A'<=word[i]&&word[i]<='Z')||('a'<=word[i]&&word[i]<='z'))
        cleaned += word[i];
    }
    for(int i=0 ; cleaned[i] ; i++){
        if(cleaned[i]>90)
        cleaned[i]-=32;
    }
    return cleaned;
}

int DicCheck(string word,Dictionary *Head,string filename){
    Dictionary *curr = Head;
    BooksNames *NewBook;
    char firstletter = word[0];
    int Final = 1;

    while(curr!=NULL){
        if(curr->Words[0]==firstletter){
            break;
        }
    curr = curr->Next;
    }
    while (curr!=NULL){
        if(curr->Words==word){
            while(curr->Books!=NULL){
                if(curr->Books->BookName==filename)
                    break;
                else{
                    NewBook = new struct BooksNames;
                    NewBook->BookName = filename;
                    NewBook->Next = curr->Books;
                    curr->Books = NewBook;
                    curr->x += 1;
                }
            }
            Final = 0;
            break;
        }
        else if(curr->Words[0]!=firstletter){
        break;
        }
        curr = curr->Next;
    }
    return Final;
}

Dictionary *SortWords(Dictionary *Head,string word){
    Dictionary *curr = Head;
    while(curr!=NULL){
        if(curr->Words>word){
        break;
        }
        curr = curr->Next;
    }
    return curr;
}

void SearchWords(Dictionary * Head){
    clock_t start,end;
    start = clock();

    int x=0;
    string SearchWord;
    Dictionary *curr = Head;    

    cout<<"Enter a word for search : ";
    cin>>SearchWord;

    for(int i=0 ; SearchWord[i] ; i++){
        if(90<SearchWord[i])
        SearchWord[i]-=32;
    }

    while(curr!=NULL){
        if (SearchWord==curr->Words){
            x=1;
            BooksNames *bcurr = curr->Books;
            cout<<"Books : "<<curr->x<<" \n";
            while(bcurr!=NULL){
                cout<<bcurr->BookName<<"\t";
                bcurr = bcurr->Next;
            }
            break;
        }
        curr = curr->Next;
    }
    if(x==0){
        cout<<"NOT FOUND!\n";
    }
    cout<<endl;

    end = clock();
    double runtime = double(end - start)/CLOCKS_PER_SEC;
    cout<<"Searching Word Time : "<<runtime<<" seconds"<<endl;
}

void Delete(Dictionary *Head){
    Dictionary *temp = Head;
    while(temp!=NULL){
        while(temp->Books!=NULL){
            delete temp->Books;
            temp->Books = temp->Books->Next;
        }
        delete temp;
        temp = temp->Next;
    }
}

void Folder(){
    string temp;
    ofstream Filename("/Users/largexim/Documents/T2AP/T5/Filename.txt");
    string path = "/Users/largexim/Documents/T2AP/T5/File";
    DIR* dir = opendir(path.c_str());
    if (dir){
        dirent* entry;
        while((entry = readdir(dir))!= nullptr) {
            temp = entry->d_name;
            if (temp[0]!='.')
            Filename << temp <<endl;
        }
        closedir(dir);
    } 
    else{
        cerr << "Error opening directory " << path <<endl;
    }
}

void test(Dictionary *Head){
    clock_t start,end;
    start = clock();

    Dictionary *curr = Head;
    string Book;
    string Word;
    bool check = false;
    char FirstLetter;
    cout<<"Enter a Book Name : ";
    getline(cin ,Book);
    getline(cin ,Book);
    cout<<"\nEnter a Word : ";
    cin>>Word;

    for(int i=0 ; Word[i] ; i++){
        if(90<Word[i])
        Word[i]-=32;
    }

    FirstLetter = Word[0];
    while(curr!=NULL){
        if(curr->Words[0]==FirstLetter)
        break;
        curr = curr->Next;
    }
    while(curr!=NULL){
        if(curr->Words==Word){
            while(curr->Books!=NULL){
                if(curr->Books->BookName==Book){
                    cout<<"yes , Mached.\n";
                    check = true;
                    break;
                }
                curr->Books=curr->Books->Next;
            }
                if(check==false){
                    cout<<"oops , Not Mached!!\n";
                    break;
                }
        }
        curr = curr->Next;
    }
    if(check==false){
        cout<<"oops , Not Mached!!\n";
    }

    end = clock();
    double runtime = double(end - start)/CLOCKS_PER_SEC;
    cout<<"Test word and Book mach Time : "<<runtime<<" seconds"<<endl;
}

