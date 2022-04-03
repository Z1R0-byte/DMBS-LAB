#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#define ll long long

using namespace std;

  
typedef struct{
    vector<int> elements;
    int size; 
    int local_depth; 
} bucket; 

vector<bucket* > directories;
int global_depth;
int bucket_capacity;
int MAX_GLOBAL_DEPTH = 20;

int hash_function(int val, int depth){
    int mod = 1<<depth; 
    return val%mod; 
}

void initializeDirectory();

bool searchBucket(int val);
void findInBucket_dev(int val);
vector<int> remove_element(vector<int> v, int val);
void deleteFromBucket(int val);
void insertIntoBucket(int val);
void rehash(int overflowLocation, int overflowValue);
void splitDirectory(int overflowValue, int prev_global_depth);
void printHashTable();

int main(){
    bucket_capacity = 2;  
    initializeDirectory();
    insertIntoBucket(0); 
    insertIntoBucket(2); 
    insertIntoBucket(1);  
    insertIntoBucket(3);
    insertIntoBucket(5); 
    // // deleteFromBucket(3);
    findInBucket_dev(7);   

    // printHashTable();  

    return 0;
}

void printHashTable(){
    for(int i =0; i< (1<< global_depth); i++){
        cout<<"Bucket no "<<i<<" contains-> ";
        for(int j = 0; j<directories[i]->elements.size(); j++){
            cout<<directories[i]->elements[j]<<" "; 
        } 
        cout<<"\n"; 
    }
}


void initializeDirectory(){
    global_depth = 1; 


    for(int i =0; i < (1<<global_depth); i++){
        bucket* bckt = new bucket;
        bckt->size = 0;
        bckt->local_depth = 1;   
        directories.push_back(bckt); 
    } 
}



bool searchBucket(int val){
    int bucket_no = hash_function(val, global_depth);

    bucket* req_bucket = directories[bucket_no]; 
    
    for(int i = 0; i<req_bucket->size; i++){
        if(req_bucket->elements[i] == val){ 
            return true; 
        }
    } 
    return false; 
}

void findInBucket_dev(int val){
    if(searchBucket(val)) cout<<"element found\n";
    else cout<<"element not found\n"; 
}

vector<int> remove_element(vector<int> v, int val){
    vector<int> ans; 
    for(int i=0; i<v.size(); i++){
        if(v[i] != val){
            ans.push_back(v[i]); 
        }
    }
    return ans; 
}

void deleteFromBucket(int val){
    int bucket_no = hash_function(val, global_depth);

    bucket* req_bucket = directories[bucket_no];

    if(searchBucket(val)){
        req_bucket->elements = remove_element(req_bucket->elements, val); 
        req_bucket->size--; 
    }
    else{
        cout<<"ELEMENT NOT PRESENT!\n"; 
    }

    if(req_bucket->size == 0){
        //merging 
    }
}

void insertIntoBucket(int val){
    int bucket_no = hash_function(val, global_depth); 
    
    bucket* req_bucket = directories[bucket_no]; 
    int n_ele = req_bucket->size;
    int loc_depth = req_bucket->local_depth;

    if(searchBucket(val)){
        cout<<"ELEMENT ALREADY PRESENT!\n";
        return;  
    }  
    // element not present

    if(n_ele < bucket_capacity){
        req_bucket->elements.push_back(val);  
        req_bucket->size = n_ele+1;
        return;   
    }
    // else

    /* if loc_depth == global depth then directory splitting must occur 
    or else we can't store the data */

    if(loc_depth == global_depth && global_depth < MAX_GLOBAL_DEPTH){
        // val is the overflow element 
        splitDirectory(val, global_depth); 
        return;  
    }
    // loc_depth < global_depth;

    cout<<"cant do that"; 
    
}

void rehash(int overflowLocation, int overflowValue){
    for(int i = 0; i< directories[overflowLocation]->elements.size(); i++){
        insertIntoBucket(directories[overflowLocation]->elements[i]); 
    }
    insertIntoBucket(overflowValue); 
}

void splitDirectory(int overflowValue, int prev_global_depth){
    int overflowLocation = hash_function(overflowValue, prev_global_depth); 
    global_depth = prev_global_depth + 1;

    for(int i = 1<<prev_global_depth ; i< (1<<global_depth); i++){
        if(hash_function(i, prev_global_depth) == overflowLocation){
            // create a new bucket
            bucket* newBucket = new bucket; 
            newBucket->size = 0; 
            newBucket->local_depth = directories[overflowLocation]->local_depth + 1; 
        }
        else{
            directories[i] = directories[hash_function(i, prev_global_depth)]; 
        }
    }

    directories[overflowLocation]->local_depth++; 

    rehash(overflowLocation, overflowValue); 
}