//
//  ByteArray.h
//  cppClient
//
//  Created by sancar koyunlu on 1/14/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//

#ifndef __cppClient__ByteArray__
#define __cppClient__ByteArray__

typedef unsigned char byte;
class ByteArray{
public:
    ByteArray(int size){
        len = size;
        buffer = new byte[size];
    };
    ByteArray(ByteArray& rhs){
        len = rhs.len;
        delete [] buffer;
        buffer = new byte[len];
        for(int i = 0; i < len; i++){
            buffer[i] = rhs.buffer[i];
        }
    };
    ~ByteArray(){
        delete [] buffer;
    };
  /*  ByteArray& operator=(const ByteArray& rhs){
        len = rhs.len;
        delete [] buffer; TODO
        buffer = new byte[len];
        for(int i = 0; i < len; i++){
            buffer[i] = rhs.buffer[i];
        }
        return *this;
    };
*/
    byte& operator[](int i){
        return buffer[i];
    };
    byte* getBuffer(){
        return buffer;
    };
    
    int length(){ return len; };
private:
    int len;
    byte* buffer;
};

#endif /* defined(__cppClient__ByteArray__) */