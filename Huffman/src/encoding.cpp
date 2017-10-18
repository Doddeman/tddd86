// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>

// TODO: include any other headers you need

map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;
    int sign;
    while(true){
        sign = input.get();
        if(sign == -1){ //EOF
            freqTable.insert(pair<int,int>(PSEUDO_EOF, 1));
            break;
        }
        else if(freqTable.find(sign) == freqTable.end()){ //If sign is not in the map
            freqTable.insert(pair<int,int> (sign, 1)); // add it
        }
        else{ //if sign is in the map
            freqTable.at(sign)++; // increase frequency
        }
    }
    return freqTable;
}

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode> prioQueue;
    HuffmanNode node;

    for (auto it = freqTable.begin(); it != freqTable.end(); it++){
        node.character = it->first;
        node.count = it->second;
        node.zero = nullptr;
        node.one = nullptr;
        prioQueue.push(node);
    }

    HuffmanNode * newNode;
    HuffmanNode first;
    HuffmanNode second;

    if(prioQueue.size() == 1){ //If empty file
        newNode = new HuffmanNode(PSEUDO_EOF, 1, nullptr, nullptr);
    }
    while(prioQueue.size() > 1){
        first = prioQueue.top();
        prioQueue.pop();
        second = prioQueue.top();
        prioQueue.pop();
        newNode = new HuffmanNode(NOT_A_CHAR,first.count+second.count,nullptr,nullptr);         //create the new node
        newNode->zero =  new HuffmanNode(first.character, first.count, first.zero, first.one);  //assign its pointers
        newNode->one = new HuffmanNode(second.character, second.count, second.zero, second.one);
        prioQueue.push(*newNode);
    }
    return newNode;
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;
    string code;
    traverse(encodingMap, encodingTree, code);
    return encodingMap;
}

void traverse(map<int, string> &encodingMap, HuffmanNode * node, string code){
    if(node == nullptr){
        return;
    }
    else if(node->isLeaf()){ //add to map
        encodingMap.insert(pair<int,string> (node->character, code));
    }
    else{ //keep traversing
        traverse(encodingMap, node->zero, code + "0");
        traverse(encodingMap, node->one, code + "1");
    }
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    string encodedData;
    int sign;
    while(true){
        sign = input.get();
        if(sign == -1){ //EOF
            encodedData += encodingMap.at(PSEUDO_EOF);
            break;
        }
        encodedData += encodingMap.at(sign);
    }
    for(unsigned int i = 0; i < encodedData.length(); i++){
        int bit = encodedData[i] - '0'; //cast to int
        output.writeBit(bit);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    string result;
    HuffmanNode * node = encodingTree;
    int bit;
    while(true){
        bit = input.readBit();
        if(bit == -1){
            break;
        }
        else if(bit == 0){
            node = node->zero;
        }
        else if(bit == 1){
            node = node->one;
        }
        if(node->isLeaf()){
            result += node->character;
            if(node->character == PSEUDO_EOF){
                break;
            }
            node = encodingTree; //return to the root
        }
    }
    for(unsigned int i = 0; i < result.length(); i++){
        output.put(result[i]);
    }
}

void compress(istream& input, obitstream& output) {
    map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(tree);

    string header = "{ ";
    for (auto it = freqTable.begin(); it != freqTable.end(); it++){
        header += to_string(it->first);
        header += ":";
        header += to_string(it->second);
        header += ", ";
    }
    header = header.substr(0,header.length()-2);
    header += "}";

    for(unsigned int i = 0; i < header.length(); i++){
        output.put(header[i]);
    }

    input.clear();
    input.seekg(0, ios::beg);
    encodeData(input, encodingMap, output);

    freeTree(tree);
}

void decompress(ibitstream& input, ostream& output) {
    map<int, int> freqTable;
    string strKey;
    string strValue;
    int key;
    int value;
    int sign;

    while(sign != '}'){
        strKey = "";
        strValue = "";
        key = 0;
        value = 0;

        sign = input.get(); //get next sign
        if(sign == '{' || sign == ' '){ //finds a key
            sign = input.get(); //skip '{' or ' ' and get first digit of key
            while(sign != ':'){
                strKey += sign;
                sign = input.get();
            }
            key = stoi(strKey);
            //cout << "key: " << key << endl;

            sign = input.get(); //skip ':' and get first digit of value
            while(sign != ','){
                if(sign == '}'){
                    break;
                }
                strValue += sign;
                sign = input.get();
            }
            value = stoi(strValue);
            //cout << "value: " << value << endl;

            freqTable.insert(pair<int,int> (key, value));
        }
    }
    HuffmanNode * tree = buildEncodingTree(freqTable);
    decodeData(input, tree, output);
}

void freeTree(HuffmanNode* node) {
    if(node == nullptr){
        return;
    }
    else if(node->isLeaf()){
        delete node;
        return;
    }
    else{
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
