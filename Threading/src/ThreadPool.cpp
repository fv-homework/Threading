#include "ThreadPool.h"

using namespace std;


ThreadPool::ThreadPool( size_t threadNumber) : _nWorker(1)
{
    //ctor
   cout << "Loading worker threads.." << endl;
   for( unsigned int i = 0; i < threadNumber ; ++ i)
        _thPool.push_back( thread( Worker(this) ));


}

ThreadPool::~ThreadPool()
{
    //dtor

    cout << "destructor activated. " << endl;

    _predicate.store( true );
    _cv.notify_all();

    for( unsigned int i = 0; i < _thPool.size(); ++i )
        _thPool.at(i).join();
}


void ThreadPool::Worker::operator()()
{

  function< void() > (task);

  while( true )
  {
       {

        unique_lock<mutex> _locker( _master->_mu );

        _master->_cv.wait( _locker,

                           [&]() { return !_master->_dequeInput.empty() || !_master->_dequeOutput.empty(); }

                         );

        if( !_master->_predicate.load() )
        {
            task = _master->_dequeInput.front();
            _master->_dequeInput.pop_front();
        }
        else if( _master->_predicate.load() )
        {
            task = _master->_dequeOutput.front();
            _master->_dequeOutput.pop_front();
        }

       } //release locke

     task();
     _master->_predicate.store( !_master->_predicate.load() );
  }
}
