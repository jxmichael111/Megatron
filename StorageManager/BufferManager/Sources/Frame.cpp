#include <string>
#include <iostream>
#include <sstream>
#include "Frame.h"

void Frame::ViewData() {
    

    for(int i = 0; i < data.size(); i++) {
        /*
        std::string FirstLine = data[i];
        size_t pos = FirstLine.find('|');
        std::string Line = FirstLine.substr(pos + 1);
        std::cout << Line << std::endl;
        //*/
        std::cout << data[i] << std::endl;
    }
}

void Frame::SetRequest(char operacion)
{   
    if (operacion == 'l' || operacion == 'w')
        operacion = operacion - ('a' - 'A');
    requests.push(operacion);
}

void Frame::ReleaseRequest() {
    if (!requests.empty()) {
        requests.pop();
    }
}

void Frame::PrintRequest() {
    std::queue<char> tempQueue = requests; 
    while (!tempQueue.empty()) {
        char entry = tempQueue.front();
        std::cout << entry << " ";
        tempQueue.pop();
    }
    std::cout<< std::endl;
}

char Frame::GetRequestFront()
{
    return requests.front();
}

Frame::Frame(int capacidad_)
{
    this->capacidad = capacidad_;
    this->isPinned = false ;
    this->dirtyFlag = false ;
    this->pinCount = 0;
    this->lastUsed = 0;
    this->refBit = false;
}

void Frame::pin() {
        isPinned = true;
}

void Frame::IncrementCount() {
    pinCount++; 
}

void Frame::DecrementCount() {
    if (pinCount > 0) {
        pinCount--; 
    }
}

void Frame::unpin() {
    isPinned = false;
}

void Frame::setDirty() {
    dirtyFlag = true;
}

void Frame::ReleaseDirty() {
    dirtyFlag = false;
}

void Frame::reset() {
    isPinned = false;
    dirtyFlag = false;
    pinCount = 0;
    lastUsed = 0;
    refBit = false;
    data.clear();
    while (!requests.empty()) {
        requests.pop();
    }
}

bool Frame::GetIsPinned() {
    return isPinned;
}

int Frame::GetLastUsed() {
    return lastUsed;
}

bool Frame::GetRefBit() {
    return refBit;
}

void Frame::SetData(std::string registro) {
    data.push_back(registro);
}

std::vector<std::string> Frame::GetData() { 
    return data;
}

void Frame::ViewRegister(int NroRegister) {
    std::string FirstLine = data[NroRegister-1];
    size_t pos = FirstLine.find('|');
    std::string aux = FirstLine.substr(pos + 1);
    

    std::cout << aux << std::endl;
    
}

std::vector<int> Frame::GetIndex(int Register) {
    std::vector<int> index;
    if (!data.empty()) {
        std::string FirstLine = data[Register-1];
        size_t pos = FirstLine.find('|');
        
        if (pos != std::string::npos) {
            std::string columnsPart = FirstLine.substr(0, pos);
            std::istringstream iss(columnsPart);
            std::string columnSize;

            while (iss >> columnSize) {
                try {
                    index.push_back(std::stoi(columnSize));

                } catch (const std::invalid_argument& e) {

                    std::cerr << "Error: valor no es un numero válido: " << columnSize << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: número fuera del rango: " << columnSize << std::endl;
                }
            }
        }
    }
    return index;
}
std::vector<std::string> Frame::GetRegister(int NroRegister) {
    std::vector<std::string> Register;
    std::vector<int> Index = GetIndex(NroRegister);

    if (NroRegister-1 >= data.size() || Index.empty()) {
        return Register; 
    }

    std::string FirstLine = data[NroRegister-1];
    size_t pos = FirstLine.find('|');

    std::string Line = FirstLine.substr(pos + 1);
    int Pos = 0;

    for (int length : Index) {
        if (Pos >= Line.size()) {
            break; 
        }

        std::string row = Line.substr(Pos,length);
        Register.push_back(row);
        Pos += length;
    }

    return Register;
}

void Frame::SetRegister(int NroRegister, std::vector<std::string> Register) {
    std::string row;
    std::string FirstLine = data[NroRegister-1];
    size_t pos = FirstLine.find('|');
    std::string Line = FirstLine.substr(0,pos+1);

    for(int i = 0; i < Register.size(); i++)
        row += Register[i];

    data[NroRegister-1]= Line + row;
}

void Frame::RemoveRegister(int NroRegister) {
    if (NroRegister < data.size()) {
        data.erase(data.begin() + NroRegister-1);
    }
}

void Frame::AddRegister(std::vector<std::string> Register) {
    std::string row;
    for(int i = 0; i < Register.size(); i++)
        row += Register[i];

    data.push_back(row);
}

bool Frame::isVoid()
{
    return data.empty();
}

bool Frame::GetDirty()
{
    return dirtyFlag;
}

int Frame::GetPinCount() {
    return pinCount;
}

void Frame::increment(){ 
    lastUsed++;
}  

void Frame::refOn() {
    refBit = true;
}

void Frame::refOff() {
    refBit = false;
}

void Frame::used(){
    lastUsed = 1;
}