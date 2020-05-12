#pragma once

/*
  <Queue with 2-stacks>

                  top                      bottom     
    stackRear  : [n - 1, n - 2, n - 3, ..., i + 1]
    stackFront : [    0,     1,     2, ..., i    ]


  1. enqueue operation : O(1)

     stackRear.push(x); // enqueue


  2. dequeue operation : O(n), but amortized O(1)

     if (stackFront.empty()) {
         while (!stackRear.empty()) {
             stackFront.push(stackRear.top());
             stackRear.pop();
         }
     }

     //result = stackFront.top();
     stackFront.pop();  // dequeue


  3. related problems
     https://www.codechef.com/problems/BINLAND

*/
