#include <iostream>
#include <vector>

using namespace std;

#define INT_SIZE 4;

class page
{
    int pageBeginning;
    int nextPageBeginning;
    int numRecords;
    int startFreeSpace;
    vector<int> records;

public:
    page(int, int);
    int searchKey(int);
    bool hasSpace(int);
    int countRecords();
    void insertKey(int, int);
    void printPage();
};

class heapFile
{
    friend class page;
    vector<page> pages;
    int currId;
    int pageSize;

public:
    heapFile(int);
    void insert(int, int);
    void search(int);
    void status();
    void print();
};

page::page(int size, int id)
{
    pageBeginning = size * id;
    nextPageBeginning = size * (id + 1);
    numRecords = 0;
    startFreeSpace = pageBeginning;
}

int page::searchKey(int key)
{
    for (int i = 0; i < records.size(); i++)
    {
        if (records[i] == key)
            return i;
    }
    return -1;
}

void page::printPage()
{
    for (int r : records)
    {
        cout << r << " ";
    }
}

bool page::hasSpace(int record_size)
{
    int freeSpace = nextPageBeginning - startFreeSpace - 16;

    if ((freeSpace - (records.size() * 4)) >= (record_size + 4))
    {
        return true;
    }
    return false;
}

int page::countRecords()
{
    return numRecords;
}

void page::insertKey(int record_size, int primary_key)
{
    startFreeSpace += record_size;
    records.push_back(primary_key);
    numRecords++;
    return;
}

heapFile::heapFile(int page_size)
{
    pages = {};
    pageSize = page_size;
    currId = 0;
}

void heapFile::status()
{
    cout << pages.size() << " ";
    for (int i = 0; i < pages.size(); i++)
    {
        cout << pages[i].countRecords() << " ";
    }
    cout << "\n";
    return;
}

void heapFile::insert(int record_size, int primary_key)
{

    // pages is not empty
    int isInserted = 0;

    for (int i = 0; i < pages.size(); i++)
    {
        if (pages[i].hasSpace(record_size))
        {
            pages[i].insertKey(record_size, primary_key);
            isInserted = 1;
            break;
        }
    }

    if (isInserted == 0)
    { // empty case is also considered here
        page p(pageSize, currId);
        p.insertKey(record_size, primary_key);
        pages.push_back(p);
        currId++;
        return;
    }
}

void heapFile::search(int primary_key)
{
    for (int i = 0; i < pages.size(); i++)
    {
        int idx = pages[i].searchKey(primary_key);
        if (idx != -1)
        {
            cout << i << " " << idx << "\n";
            return;
        }
    }
    cout << -1 << " " << -1 << "\n";
    return;
}

void heapFile::print()
{
    for (int i = 0; i < pages.size(); i++)
    {
        pages[i].printPage();
    }
}
int main()
{
    int pSize;
    cin >> pSize;
    heapFile h(pSize);

    int input;

    while (true)
    {
        cin >> input;
        if (input == 1)
        {
            int recSize, pKey;
            cin >> recSize >> pKey;
            h.insert(recSize, pKey);
        }
        else if (input == 2)
        {
            h.status();
        }
        else if (input == 3)
        {
            int keyToBeSearched;
            cin >> keyToBeSearched;
            h.search(keyToBeSearched);
        }
        else if (input == 4)
        {
            break;
        }
        else
        {
            cout << "ERROR: Wrong input, please try again.\n";
        }
    }

    return 0;
}