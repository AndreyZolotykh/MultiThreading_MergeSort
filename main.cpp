#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <cassert>

void merge(int* arr, int l, int m, int r)
{
   int nl = m - l + 1;
   int nr = r - m;

   // создаем временные массивы
   int left[nl], right[nr];

   // копируем данные во временные массивы
   for (int i = 0; i < nl; i++)
       left[i] = arr[l + i];
   for (int j = 0; j < nr; j++)
       right[j] = arr[m + 1 + j];

   int i = 0, j = 0;
   int k = l;  // начало левой части

   while (i < nl && j < nr) {
       // записываем минимальные элементы обратно во входной массив
       if (left[i] <= right[j]) {
           arr[k] = left[i];
           i++;
       }
       else {
           arr[k] = right[j];
           j++;
       }
       k++;
   }
   // записываем оставшиеся элементы левой части
   while (i < nl) {
       arr[k] = left[i];
       i++;
       k++;
   }
   // записываем оставшиеся элементы правой части
   while (j < nr) {
       arr[k] = right[j];
       j++;
       k++;
   }
}

void mergeSort( int array[], size_t l, size_t r )
{
    if ( l >= r )
        return;
    
    int m = l + ( r - l ) / 2;
    
    if ( r - l > 100'000 )
    {
        auto futureLeft = std::async( std::launch::async, [&]()
        {
            mergeSort( array, l, m );
        });
        
        auto futureRight = std::async( std::launch::async, [&]()
        {
            mergeSort( array, m + 1, r );
        });
    }
    else
    {
        mergeSort( array, l, m );
        mergeSort( array, m + 1, r );
    }
        
    merge( array, l, m, r );
}

int main(int argc, const char * argv[]) {
    std::vector<int> v( 1'000'000 );
    std::generate( v.begin(), v.end(), std::rand );

    std::vector<int> copy = v;

    using namespace std::chrono;

    std::cout << "Single thread" << std::endl;

    auto begin = system_clock::now();
 
    std::sort( copy.begin(), copy.end() );
    
    auto end = system_clock::now();
 
    std::cout << "The time: " << duration_cast<milliseconds>(end - begin).count() << " milliseconds" << std::endl << std::endl;
 
    std::cout << "Multithread" << std::endl;

    begin = system_clock::now();
 
    mergeSort( v.data(), 0, v.size() - 1 );
 
    end = system_clock::now();
 
 
    std::cout << "The time: " << duration_cast<milliseconds>(end - begin).count() << " milliseconds" << std::endl;
 
    
    assert( copy == v );
    
    std::cout << "Done" << std::endl;
    
    return 0;
}
