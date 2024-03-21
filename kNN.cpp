#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

// Implement DLIST CLASS

template <typename T>
DList<T>::~DList(){
    this -> clear();
}
template <typename T>
void DList<T>::push_back(T value) {
    Node* newNode = new Node(value);
    if (this -> size == 0) {
        this -> head = this -> tail = newNode;
    }
    else {
        this -> tail -> next = newNode;
        newNode -> prev = this -> tail;
        this -> tail = this -> tail -> next;
    }
    this -> size++;
}
template <typename T>
void DList<T>::push_front(T value) {
    Node* newNode = new Node(value);
    if ( this -> size == 0) {
        this -> head = this -> tail = newNode;
    }
    else {
        newNode -> next = this -> head;
        this -> head -> prev = newNode;
        this -> head = newNode;
    }
    this -> size++;
}
template <typename T>
void DList<T>::insert(int index, T value) {
    if (index < 0 || index > size) return;
    if ( index == 0){ 
        this -> push_front(value);
    }
    else if (index == this -> size ) {
        this -> push_back(value);
    }
    else {
        Node* newNode = new Node(value);
        Node* current = this -> head;
        for (int i = 0; i < index -1 ; ++i)
            current = current -> next;
        newNode -> next = current -> next;
        newNode -> prev = current;
        current -> next -> prev = newNode;
        current -> next = newNode;
        this -> size++;
    }

}
template <typename T>
void DList<T>::remove(int index) {
    if ( index < 0 || index >= this -> size) return;
    if (index == 0){
        Node* ptr = this -> head;
        if (this -> size == 1) this -> head = this -> tail = NULL;
        else {
            this -> head = this -> head -> next;
            this -> head -> prev = NULL;
        }
        delete ptr; 
    }
    else if (index ==  this -> size - 1){
        Node* ptr = this -> tail;
        this -> tail = this -> tail->prev;
        this -> tail->next = nullptr;
        delete ptr;    
    }
    else {
        Node* ptr = this -> head;
        for (int i = 0; i < index ; ++i)
            ptr = ptr -> next;
        ptr -> next -> prev = ptr -> prev;
        ptr -> prev -> next = ptr -> next;
        delete ptr;
    }
    this -> size --;
    return;

}
template <typename T>
T & DList<T>::get(int index) const {
    if(index < 0 || index >= this->size)  throw std::out_of_range("get(): Out of range");
        
    if (index <= size/2){
        Node* ptr = this -> head;
        for (int i = 0; i < index; ++i)
            ptr = ptr -> next;
        return ptr -> data;
    }
    else {
        Node* ptr = this -> tail;
        for (int i = size - 1; i > index; i--)
            ptr = ptr -> prev;
        return ptr -> data;
    }
            //TODO: implement task 1  
            // return head->data;
}
template <typename T>
int DList<T>::length() const {
    return this -> size;
}
template <typename T>
void DList<T>::clear() {
    if (this -> size == 0) return;
    while (this -> head != NULL){
        Node* removedNode = this -> head;
        this -> head = this -> head -> next;
        delete removedNode;
    }
    this -> size = 0;
    this -> head = this -> tail = NULL;
}
template <typename T>
void DList<T>::print() const {
    if (this -> size == 0) return;
    Node* ptr = this -> head;
    for (int i =0 ; i < this -> size - 1; ++i){
        cout<<ptr->data<<" ";
        ptr = ptr -> next;
    }
    cout<<ptr->data;
}
template <typename T>
void DList<T>::reverse() {
    if (this -> size == 0 || this -> size == 1) return;

    Node* current = this -> head;
    Node* temp = nullptr;

    // Swap next and prev pointers for all nodes of the doubly linked list
    while (current != nullptr) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }

    // If the current is null, it means we've reached the end of the list
    // So, the new head will be the previous node of the previous head

    // Swap head and tail pointers
    temp = this -> head;
    this -> head = this -> tail;
    this -> tail = temp;
}


/*IMPLEMENT CLASS DATASET*/
Dataset::Dataset() : data(new DList<List<int>*>()), columnNames(new DList<std::string>()), col(0), row(0) {}

Dataset::Dataset(const Dataset& other){ 
    this -> row = other.row;
    if (this -> row == 0) this -> col = 0;
    else this -> col = other.col;
    //deep copy for data 
    data = new DList<List<int>*>();
    for (int i = 0; i < other.data->length(); ++i){
        List<int>* dataRow = new DList<int>;
        for (int j = 0; j < other.data->get(i)->length(); ++j)
          dataRow->push_back(other.data->get(i)->get(j));
        data -> push_back(dataRow);
    }
    //deep copy for columnNames
    columnNames = new DList<string>();
    for (int i = 0; i < other.columnNames->length(); ++i)
        columnNames->push_back(other.columnNames->get(i));
    
}
Dataset Dataset::extract(int startRow , int endRow , int startCol , int endCol ) const{
    if (endRow < -1 || endCol < -1 || startRow < 0 || startCol < 0 ||
    ( startRow > endRow  && endRow != -1)|| (startCol > endCol && endCol != -1))
        throw std::out_of_range("get(): Out of range");
    if (endRow == - 1 || endRow >= row) endRow = row - 1;
    if (endCol == - 1 || endCol >= col) endCol = col - 1;
    Dataset extractedTable;
    if (startRow > endRow || startCol > endCol) return extractedTable;
    List<string>* extractedName = new DList<string>();
    List<List<int>*>* extractedData = new DList<List<int>*>();
    // ########------------------------- Load data ------------------------------------------------------ 100%
    //
    int ERow = 0;
    int ECol = 0;
    for (int i = startRow; i <= endRow; ++i ){
        DList<int>* extractedRow = new DList<int>();
        for (int j = startCol; j <= endCol; ++j){
            extractedRow->push_back(data -> get(i) -> get(j));       
        }
        extractedData-> push_back(extractedRow);
        ERow++;
    }
    // ########------------------------- Load name line ------------------------------------------------------ 100%
    for (int j = startCol; j <= endCol; ++j){
        extractedName -> push_back(columnNames->get(j));
        ECol++;
    }
    extractedTable.data = extractedData;
    extractedTable.columnNames = extractedName;
    extractedTable.col = ECol;
    extractedTable.row = ERow;
    return extractedTable;
}
bool Dataset::drop(int axis , int index , std::string columns ){
  // index is start the line which below name line
  //delete row
  if (axis == 0) 
    {
        if (index >= row || index < 0) return false;
        else {
            data -> remove(index);
            row --;
            if (row == 0) col = 0;
        }
        return true;
    }
  //delete col
  else if (axis == 1){
        for (int j = 0; j < col; ++j){
          if (columnNames->get(j) == columns) {
              columnNames -> remove(j);
              for (int i = 0 ; i < data->length() ; i++ ) {
                data -> get(i)->remove(j);
              }
              col --;
              return true;
          }
        }
        return false;
  }
  //   if (axis!=0 && axis!=1) 
  return false;
}
Dataset::~Dataset(){
    if (this -> data == nullptr && this -> columnNames == nullptr) return;
    this -> columnNames -> clear();
    delete this -> columnNames;
    for (int i=0;i < this -> data -> length(); i++)
        this -> data -> get(i) -> clear();
    delete this -> data;
    this -> col = this -> row = 0;
}
Dataset& Dataset :: operator=(const Dataset& other){
    if (this == &other){
      return * this;
    }
    delete this -> columnNames;
    for (int i=0;i < this -> data -> length(); i++)
        this -> data -> get(i) -> clear();
    delete this -> data; 
    ////////////////////////
   //deep copy for data
    data = new DList<List<int>*>(); 
    this -> row = other.row; 
    if (this -> row == 0) this -> col = 0;
    else this -> col = other.col;
    for (int i = 0; i < other.data->length(); ++i){
        List<int>* dataRow = new DList<int>;
        for (int j = 0; j < other.data->get(i)->length(); ++j)
          dataRow->push_back(other.data->get(i)->get(j));
        data -> push_back(dataRow);
    }
    //deep copy for columnNames
    columnNames = new DList<string>();
    for (int i = 0; i < other.columnNames->length(); ++i)
        columnNames->push_back(other.columnNames->get(i));
    return *this; 
    
}

bool Dataset::loadFromCSV(const char* fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    string line;
    getline(file, line);
    stringstream ss(line);

    string token;
    while (getline(ss, token, ',')) {
        columnNames->push_back(token);
        col++;
    }
    while (getline(file, line)) {
        if (!line.empty()){
            ss.clear();
            ss.str(line);
            DList<int>* dataRow = new DList<int>();
            int val;
            // string number;
            stringstream tmp;
            while (getline(ss, token, ',')) {
                tmp.clear();
                tmp.str(token);
                tmp >> val;
                dataRow->push_back(val);
            }
            data->push_back(dataRow);
            row++;
        }
    }
    if (row == 0) col = 0;
    return true;
}
void Dataset::printHead(int nRows, int nCols) const {
    
    nRows = min(nRows, row);
    nCols = min(nCols, col);
    int title;
    if (nCols > 0) title = nCols;
    else title = columnNames -> length();
    //print line name
    for (int j = 0; j < title; ++j) {
        cout << columnNames->get(j) ;
        if (j < title - 1) cout<<" ";
    }
    cout<<endl;
    if(nRows <= 0 || nCols <= 0) return;
    //name line

    for (int i = 0; i < nRows ; ++i) {
        List<int>* rowData = data->get(i);
        for (int j = 0; j < nCols; ++j) {
            cout << rowData->get(j);
            if (j < nCols - 1) cout<<" ";
        }
        if (i<nRows-1)cout << endl;
    }
}
void Dataset::printTail(int nRows , int nCols ) const{  
    nRows = min(nRows, row);
    nCols = min(nCols, col);
    int title;
    if (nCols > 0) title = nCols;
    else title = columnNames -> length();
    //print name line
    for (int j = columnNames->length() - title  ; j < columnNames->length(); ++j) {
        cout << columnNames->get(j) ;
        if (j < columnNames->length() - 1) cout<<" ";
    }
    cout<<endl;
    if(nRows <= 0 || nCols <= 0)  return;

    //print other lines
    for (int i= row - nRows ; i < row ; ++i){
        List<int>* rowData = data->get(i);
        for (int j= col - nCols ; j < col;++j){
            cout << rowData->get(j) ;
            if (j < col - 1) cout<<" ";
        }
        if (i<row-1)cout << endl;
    }    
}
void Dataset::getShape(int& nRows, int& nCols) const{
  nRows = this -> row;
  nCols = this -> col;
}
void Dataset::columns() const{
  this->columnNames->print();
}
List<List<int>*>* Dataset::getData() const{
  return this -> data;
}

/*METHOD BONUS FOR DATASET CLASS TO USE IN kNN*/
void Dataset::setData( List<List<int>*>* data){
    this -> data = data;
    this -> row = data->length();
    if (this -> row == 0) this -> col = 0;
}
void Dataset::setName(List<string>* Name){
    this -> columnNames = Name;
    if (this -> row == 0) this -> col = 0;
    else this -> col = Name->length();
}

/*Function bonus*/
void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test){
  // X: feature
  // y: label  
  // Dataset feature = dataset.extract(0, -1, 1, -1);
  // Dataset label = dataset.extract(0, -1, 0, 0);     
  /*
    *test_size = 0.2 
    *size 199x784
    -> test 40x784
    -> train 159x784
  */  
        if (test_size >= 1 || test_size <= 0)
        {
            return;
        }
        int nRowsX, nColsX,nRowsY,nColsY;
        X.getShape(nRowsX, nColsX);
        y.getShape(nRowsY,nColsY);
        if(nRowsX == 0 && nColsX == 0) {
            nRowsY = 0;
            nColsY = 0;
            return;
        }
        int sizeTrainX =  floor((1-test_size)*nRowsX);  
        int sizeTestX = nRowsX - sizeTrainX; 
        int sizeTrainY =  floor((1-test_size)*nRowsY);  
        int sizeTestY = nRowsY - sizeTrainY; 
        if (sizeTestX == nRowsX && sizeTestY == nRowsY) {
            sizeTrainX = 1;
            sizeTrainY = 1;
        }
        // Begin extract
        X_train = X.extract(0,sizeTrainX-1,0,-1);
        X_test = X.extract(nRowsX-sizeTestX,nRowsX-1,0,-1);
        y_train = y.extract(0,sizeTrainY-1,0,-1);                
        y_test = y.extract(nRowsY-sizeTestY,nRowsY-1,0,-1);
        }

        
void insertionSort(double* distance, int count, int* k_top, int k){ 
    if (count == 1){
        k_top[0] = 0;
        return;
    }
    else if (count > 1 ){
        int curr = 1;
        for (int i = 0; i < k; ++i) k_top[i]=i;
        while (curr < count){
            double tmp = distance[curr];
            int step = curr - 1; 
            while (step >= 0 && tmp < distance[step]){
                distance[step+1] = distance[step];
                step--;
            }        
            distance[step+1] = tmp;
            // /////////////////// bonus
            int in_k = step + 1;

            // /// maping value in k_top to index in distance 
            // // value: index y_train -> label
            if (in_k < k){
                // move item in k_top from in_k to k-2 to right
                for (int i = k - 2; i >= in_k; --i) {
                    k_top[i + 1] = k_top[i];
                }
                // update the index in to in_k
                k_top[in_k] = curr;
            }  
            curr++; 
      }

    }
}

double calDistance(List<int>* sample, List<int>* train ){
    int size = sample -> length();
    double sum = 0.0;
    double diff;
    for (int i = 0; i< size; ++i){
        diff = sample->get(i) - train->get(i);
        sum+= diff*diff;
    }
    return sqrt(sum);
}





/* IMPLEMENT kNN*/
kNN::kNN(int k){
    this -> k = k;
}
void kNN::fit(const Dataset& X_train, const Dataset& y_train){
    this->X_train = X_train;
    this ->y_train = y_train;
}

Dataset kNN::predict(const Dataset& X_test){
    // calculate distance from each test to all train
    Dataset y_pred;
    double* distances;
    int mnist[10];
    int test_axisY,test_axisX;
    int train_axisY,train_axisX;
    X_train.getShape(train_axisY,train_axisX);
    X_test.getShape(test_axisY,test_axisX);
    if (test_axisY == 0 || test_axisX == 0) {
        return y_pred;
    }
    ///////////////////////////////////////////
    if (k > train_axisY) {
        // k = train_axisY;
        throw std::out_of_range("get(): Out of range");
    }
    int* k_top = new int[k];
    List<List<int>*>* dataTest = X_test.getData();
    List<List<int>*>* dataTrain = X_train.getData();
    List<List<int>*>* labelTrain = y_train.getData();
    List<List<int>*>* labelTestData = new DList<List<int>*> ();
    List<string>* labelTestName = new DList<string>();
    labelTestName->push_back("label");
    for (int i=0; i < test_axisY; ++i){
        List<int>* sample = dataTest -> get(i);
        distances = new double[train_axisY];
        for (int j = 0; j < train_axisY; ++j){
            List<int>* train = dataTrain -> get(j);
            double Distance = calDistance(sample,train);
            distances[j] = Distance;
        }
        insertionSort(distances,train_axisY,k_top,k);
        // getLabel predict for sample
        // create array mnist for frequency of label 
        for (int j = 0; j < 10; ++j) mnist[j] = 0;
        // count the frequency and save into array mnist
        for (int j = 0; j < k; ++j){
            int index = k_top[j];
            int label = labelTrain -> get(index) -> get(0);
            mnist[label]++;
        }
        //find the label that has the most frequency
        int max = mnist[0];
        int predLabel = 0;
        for (int j = 1; j < 10; ++j){
            if (max < mnist[j]) {
                max = mnist[j];
                predLabel = j;
            }
        }
        // push into the rowLabel
        List<int>* Label = new DList<int>();
        Label -> push_back(predLabel);
        labelTestData -> push_back(Label);
       delete[] distances;
    } 
    // set data and name to Dataset y_pred;
    y_pred.setData(labelTestData);
    y_pred.setName(labelTestName);
    delete[] k_top;
    return y_pred;
}

double kNN::score(const Dataset& y_test, const Dataset& y_pred){
    double accuracy;
    List<List<int>*>* dataTest = y_test.getData();
    List<List<int>*>* dataPred = y_pred.getData();
    int sampleSize = dataPred -> length();
    if (sampleSize == 0) return -1;
    int count = 0;
    for (int i = 0; i < sampleSize; ++i){
        if (dataTest->get(i)->get(0) == dataPred->get(i)->get(0))
        count++;
    }
    accuracy = static_cast<double>(count)/sampleSize;
    return accuracy;
}


