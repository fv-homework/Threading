//
//  main.cpp
//  Threading
//
//  Created by Federico Virgili on 16/09/19.
//  Copyright © 2019 Federico Virgili. All rights reserved.
//

#include <iostream>
#include "ThreadPool.h"


using namespace std;

void fooInput( int nCount)
{
    for( int i = 0; i < nCount; ++i )
        printf("%p - Input: %d\n",this_thread::get_id(),i );
    
    printf("\n" );
    
}

void fooOutput( int nCount)
{
    for( int i = 0; i < nCount; ++i )
        printf("%p - Output: %d\n",this_thread::get_id(),i );
    
    printf("\n" );
    
}

int main()
{
    
    ThreadPool tp(8);
    
    while(1)
    {
        
        tp.enqueueSend( std::bind(fooInput, 7) );
        sleep(1);
        
        tp.enqueueReceive( std::bind(fooOutput, 9) );
        sleep(1);
        
    }
    
    
    return 0;
}

