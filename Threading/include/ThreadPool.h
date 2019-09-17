#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <memory>
#include <thread>
#include <functional>
#include <condition_variable>
#include <algorithm>
#include <mutex>
#include <queue>
#include <future>
#include <iostream>
#include <unistd.h>


using namespace std;



class ThreadPool
{


    public:
        ThreadPool( size_t threadNumber );
        virtual ~ThreadPool();

        ThreadPool(const ThreadPool& other) = default;
        ThreadPool( ThreadPool && ) = default;


        class Worker
        {

        public:
            Worker( ThreadPool *pool ) : _master( pool ) {}

            void operator()();

        private:
            ThreadPool
                *_master
            ;


        };

        template<class F>
        void enqueueSend( F f )
        {

            unique_lock< mutex > _locker( _mu );
            _cv.wait( _locker , []() { return true;} );

            cout << "Loading Input function.." << endl;
            _dequeInput.push_back( std::function<void()>(f) );


            //_predicate.store( true );

            _cv.notify_one();

        }

         template<class F>
        void enqueueReceive( F f )
        {

            unique_lock< mutex > _locker( _mu );
            _cv.wait( _locker , []() { return true;} );

            cout << "Loading Output function.." << endl;
            _dequeOutput.push_back( std::function<void()>(f));


            //_predicate.store( true );

            _cv.notify_one();

        }



    private:

       friend class Worker;

       mutex
                                _mu
      ;

       condition_variable
                                _cv
      ;

       atomic_bool
                                _predicate{false};
       ;

       vector<thread>
                                _thPool
       ;

       deque<function<void()>>
                                _dequeInput, _dequeOutput
       ;

       int _nWorker;

};

#endif // THREADPOOL_H
