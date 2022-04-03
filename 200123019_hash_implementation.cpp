#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#define ll long long

using namespace std;

typedef struct {
    vector<int> elements; 
    int local_depth; 
} bucket; 

vector<bucket*> directory;
vector<int> bucket_creation;
int global_depth;
int MAX_GLOBAL_DEPTH = 20;
int bucket_size;   

int id_hash(int val, int depth){
    int mod = 1<<depth; 
    return val%mod; 
}

void initialize(int depth); 
bool search(int val); 
bool insert(int val);
bool deleteVal(int val);  
void split(bucket* to, int val);
void double_directory(int overflow_val);
void reinsert(bucket* to, int val);   
void printHashTable();
void displayStatus(); 
vector<int> remove_element(vector<int> v, int val);

int main()
{
    cin >> global_depth >> bucket_size;
    initialize(global_depth); 
    bool flag = false; 

    while (true)
    {
        if(flag)
        {
            cout << "Exiting from program. \n";
            break;
        }
        cout<<"\n";
        int instruction;
        cin >> instruction;
        switch (instruction)
        {
        case 2:
        {
            // insert a new value
            int value;
            cin >> value;
            bool res = insert(value);
            if(res)
            {
                cout << "Key Inserted\n";
            }
            break;
        }
        case 3:
        {
            int value;
            cin >> value;
            if(search(value)){
                cout << value << " was found\n"; 
            }
            else{
                cout << value << " was not  found\n";
            }

            break;
        }
        case 4:
        {
            int value;
            cin >> value;
            bool res = deleteVal(value);
            if(res)
            {
                cout << "Key Deleted\n";
            }
            break;
        }
        case 5:
        {   
            // printHashTable(); 
            displayStatus(); 
            break;
        }
        case 6:
        {
            flag = true;
            break;
        }
        default:
        {
            cout << "Not a valid input. Please try again\n";
        }
        }
    }

    return 0;
}

bool search(int val){
    // the directory location containing the value
    int location = id_hash(val, global_depth); 

    // pointer to the bucket to be searched
    bucket* bkt_ptr = directory[location]; 
    
    // search through all of the elements of the bucket
    for(int i =0; i<bkt_ptr->elements.size(); i++){
        if(bkt_ptr->elements[i] == val){
            // if value if found return true
            return true; 
        }
    }

    // element is not found hence return false 
    return false; 
}


void initialize(int depth){
    global_depth = depth;
    int size = 1<<depth;
    directory.resize(size);
       
    for(int i = 0; i< size; i++){
        bucket* temp = new bucket; 
        temp->local_depth = global_depth; 
        directory[i] = temp;
        bucket_creation.push_back(i);  
    }
}

bool insert(int val){
    // we won't store any duplicates
    if(search(val)){
        cout<<"ELEMENT IS ALREADY PRESENT.\n"; 
        return false;
    }

    // it is not a duplicate hence we have to take care of it's insertion
    int location = id_hash(val, global_depth); 

    // "to" is the pointer to the bucket we insert the element into 
    bucket* to = directory[location];

    // if bucket has space it is easy insertion 
    if(to->elements.size() < bucket_size ){
        to->elements.push_back(val);
        return true;  
    }

    // if local_depth< global_depth then the splitted version was already created,
    // we just need to reinsert all of the elements
    if(to->local_depth < global_depth){
        split(to, val); 
        reinsert(to, val); 
        return true;
    } 
    
    // else splitting must happen
    // there are two types. 
    // if the local depth == global depth, then directory is doubled; 
    if(to->local_depth == global_depth && global_depth < MAX_GLOBAL_DEPTH){
        // double the directory and reinsert the whole bucket 
        // keep on doing this recursively 
        double_directory(val);
        reinsert(to, val); 
        return true; 
    } 

    if(to->local_depth == global_depth && global_depth >= MAX_GLOBAL_DEPTH){
        // double the directory and reinsert the whole bucket 
        // keep on doing this recursively 
        cout<<"WE HAVE REACHED THE LIMIT, INSERTING THIS VALUE IS NOT POSSIBLE\n"; 
        return false; 
    }

    return false; 
}

void double_directory(int overflow_val){
    int prev_global_depth = global_depth; 
    global_depth++;
    int new_size = 1<<global_depth; 
    int old_size = 1<<prev_global_depth;
    directory.resize(new_size);

    for(int i = 0; i<old_size; i++){
        directory[old_size+i] = directory[i]; 
    }

    // the location that caused the overflow should be the previous location that the 
    // current value mapped to 
    int overflow_loc = id_hash(overflow_val, prev_global_depth); 
    bucket* temp = new bucket; 
    temp->local_depth = global_depth; 
    directory[overflow_loc + old_size] = temp;
    bucket_creation.push_back(overflow_loc + old_size); 

    // the previous overflow_loc can't be determined on the basis of its local depth
    // we now have to look at it's global depth 

    directory[overflow_loc]->local_depth = global_depth;    
}

void printHashTable(){
    for(int i =0; i<bucket_creation.size(); i++){
        int bucket_no = bucket_creation[i];
        cout<<"Bucket no "<<bucket_no<<" contains-> ";
        for(int j = 0; j<directory[bucket_no]->elements.size(); j++){
            cout<<directory[bucket_no]->elements[j]<<" "; 
        } 
        cout<<"\n"; 
    }
}

void displayStatus(){
    cout<<global_depth<<"\n";
    cout<<bucket_creation.size()<<"\n"; 
    for(int i =0; i<bucket_creation.size(); i++){
        int bucket_no = bucket_creation[i];
        cout<<directory[bucket_no]->elements.size()<<" "; 
        cout<<directory[bucket_no]->local_depth<<"\n"; 
    }
}

void split(bucket* to, int val){
    // since the position of the value is determined by the local depth
    // although the global depth is greater than local depth
    int local_loc = id_hash(val, to->local_depth); 
    bucket* temp = new bucket; 
    temp->local_depth = to->local_depth+1; 
    directory[local_loc+ (1<<to->local_depth)] = temp;
    // as we have created a new bucket 
    bucket_creation.push_back(local_loc+ (1<<to->local_depth));
    to->local_depth++;
}

void reinsert(bucket* to, int val){
    // clears all of the element in the current bucket
    // and inserts reinserts them along with the overflow value 

    vector<int> buffer = to->elements; 
    buffer.push_back(val); 

    to->elements.clear();

    for(int i = 0; i<buffer.size(); i++){
        insert(buffer[i]); 
    }
}

bool deleteVal(int val){
    if(!search(val)){
        cout<<"ELEMENT NOT FOUND!\n";
        return false;
    }
    // if we reach up to this point we have to delete it

    int location = id_hash(val, global_depth); 

    // from is the bucket we have to delete from 
    bucket* from = directory[location]; 

    from->elements = remove_element(from->elements, val); 

    return true; 
    // merging will be handled later. 
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


