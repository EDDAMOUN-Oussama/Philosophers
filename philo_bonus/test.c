// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>

// // Function that each thread will execute
// void* print_message(void* thread_id) {
//     long tid = (long)thread_id;
//     sleep(5);  // Simulating some work being done
//     printf("Thread %ld is executing\n", tid);
//     sleep(5);  // Simulating some work being done
//     printf("Thread %ld is done\n", tid);
//     return NULL;
// }

// int main() {
//     pthread_t threads[5];  // Create an array to hold thread IDs
//     int rc;
    
//     // Create 5 threads
//     for (long t = 0; t < 5; t++) {
//         printf("Main: Creating thread %ld\n", t);
//         // sleep(1);  // Simulating some work being done
//         rc = pthread_create(&threads[t], NULL, print_message, (void*)t);
//         if (rc) {
//             printf("Error creating thread %ld\n", t);
//             return -1;
//         }
//     }

//     // Wait for all threads to finish
//     for (long t = 0; t < 5; t++) {
//         pthread_join(threads[t], NULL);
//     }

//     printf("Main: All threads are finished\n");
//     return 0;
// }


// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <sys/time.h>

// pthread_mutex_t mutex;  // Mutex declaration

// void* thread_function(void* arg) {
//     long tid = (long)arg;
//     struct timeval tv;

//     // Get the current time 
//     sleep(2 * (unsigned int) arg);  // Simulating work
//     gettimeofday(&tv, NULL);
//     printf("Thread %ld start time: %ld.%06ld\n", tid, tv.tv_sec, tv.tv_usec);

//     // Lock the mutex before entering the critical section
//     pthread_mutex_lock(&mutex);

//     // Simulate some work
//     printf("Thread %ld is working\n", tid);
//     sleep(2 * (unsigned int) arg);  // Simulating work

//     // Get the time after work is done
//     gettimeofday(&tv, NULL);
//     printf("Thread %ld end time: %ld.%06ld\n", tid, tv.tv_sec, tv.tv_usec);

//     // Unlock the mutex after work
//     pthread_mutex_unlock(&mutex);

//     return NULL;
// }

// int main() {
//     pthread_t threads[3];  // Array to store thread IDs
//     int rc;
//     long t;

//     // Initialize the mutex
//     pthread_mutex_init(&mutex, NULL);

//     // Create threads
//     for (t = 0; t < 3; t++) {
//         printf("Main: Creating thread %ld\n", t);
//         rc = pthread_create(&threads[t], NULL, thread_function, (void*)t);
//         if (rc) {
//             printf("Error in creating thread %ld\n", t);
//             return -1;
//         }
//     }

//     // Detach the threads (no need to join because we are detaching)
//     for (t = 0; t < 3; t++) {
//         pthread_detach(threads[t]);
//     }

//     // Wait for threads to finish (without using join here as we detached them)
//     sleep(3);

//     // Destroy the mutex after usage
//     pthread_mutex_destroy(&mutex);

//     printf("Main: All threads are finished\n");

//     return 0;
// }



// # include  <unistd.h>
//  # include  <stdio.h>
//  # include  <pthread.h>

// int balance = 0;

// // // اكتب الرصيد الجديد (بعد تأخير محاكاة لمدة 1/4 ثانية) 
// void  write_balance ( int new_balance)
//  { 
//   usleep( 250000 ); 
//   balance = new_balance; 
// } 

// // إرجاع الرصيد (بعد تأخير محاكاة لمدة 1/4 ثانية) 
// int  read_balance ()
//  { 
//   return balance; 
// } 

// // تنفيذ إيداع 
// void * deposit ( void *amount)
//  { 
//   int account_balance = read_balance(); 

//    account_balance += *(( int *) amount); 
//    write_balance(account_balance); 
//     // for(int i = 0; i < 4080; i++)
//     // balance++;
//   return  NULL;
// } 



// int  main ()
//  { 



//   pthread_t thread1; 
//   pthread_t thread2; 


//   int deposit1 = 300;
//   int deposit2 = 200;


//    pthread_create(&thread1, NULL , deposit, ( void *) &deposit1); 
//   pthread_create(&thread2, NULL , deposit, ( void *) &deposit2); 


//    pthread_join(thread1, NULL ); 
//   pthread_join(thread2, NULL ); 



//   int after = read_balance(); 
//   printf ( "After: %d\n" , balance); 

//     return (0);
//  }



// # include  <unistd.h>
//  # include  <stdio.h>
//  # include  <pthread.h>

//  // الرصيد الأولي هو 0 
// int balance = 0 ; 
// pthread_mutex_t mutex; 

// // اكتب الرصيد الجديد (بعد تأخير محاكاة لمدة 1/4 ثانية) 
// void  write_balance ( int new_balance)
//  { 
//   usleep( 250000 ); 
//   balance = new_balance; 
// } 

// // إرجاع الرصيد (بعد تأخير محاكاة لمدة 1/4 ثانية) 
// int  read_balance ()
//  { 
//   usleep( 250000 ); 
//   return balance; 
// } 

// // تنفيذ إيداع 
// void * deposit ( void *amount)
//  { 
//   // قفل المزامنة المتبادلة
//    pthread_mutex_lock(&mutex); 

//   // استرداد رصيد البنك 
//   int account_balance = read_balance(); 

//   // قم بالتحديث محليًا
//    account_balance += *(( int *) amount); 

//   // اكتب رصيد البنك الجديد
//    write_balance(account_balance); 

//   // إلغاء القفل لجعل القسم الحرج متاحًا لخيوط أخرى
//    pthread_mutex_unlock(&mutex); 

//   return  NULL ; 
// } 

// int  main ()
//  { 
//   // متغير المزامنة المتبادلة 

//   // إخراج الرصيد قبل الودائع 
//   int before = read_balance(); 
//   printf ( "Before: %d\n" , before); 

//   // سننشئ خيطين لإجراء إيداع باستخدام دالة الإيداع 
//   pthread_t thread1; 
//   pthread_t thread2; 

//   // تهيئة المزامنة المتبادلة
//    pthread_mutex_init(&mutex, NULL ); 

//   // مبالغ الإيداع... يجب أن يكون الإجمالي الصحيح بعد ذلك 500 
//   int deposit1 = 300 ; 
//   int deposit2 = 200 ; 

//   // إنشاء خيوط لتشغيل دالة الإيداع بهذه المبالغ المودعة
//    pthread_create(&thread1, NULL , deposit, ( void *) &deposit1); 
//   pthread_create(&thread2, NULL , deposit, ( void *) &deposit2); 

//   // الانضمام إلى الخيوط
//    pthread_join(thread1, NULL ); 
//   pthread_join(thread2, NULL ); 

//    // تدمير المزامنة المتبادلة
//    pthread_mutex_destroy(&mutex);

//   // إخراج الرصيد بعد الودائع 
//   int after = read_balance(); 
//   printf ( "After: %d\n" , after); 

//   return  0 ; 
// }


// pthread_mutex_t p;
// pthread_mutex_t *p1;
// pthread_mutex_t *p2;


// p1 = &p;
// p2 = &p;
// pthread_mutex_init(p);
// pthread_mutex_lock(p1, NULL);
// pthread_mutex_lock(p2, NULL);














#include <semaphore.h>
#include <fcntl.h>   
#include <sys/stat.h>
#include <unistd.h>  
#include <stdio.h>   
#include <stdlib.h>  


int main()
{
    sem_t *tiket;

    tiket = sem_open("/tiket", O_CREAT, 0644, 3);
    pid_t p[10];
    int i = 0;
    int some = 0;
    int *ptr = &some;

    while (i < 10)
    {
        p[i] = fork();
        if (p[i] == 0)
        {
            int id = i;
            sem_wait(tiket);
            printf("the personne %d has enter\n", id + 1);
            sleep(2);
            (*ptr)++;
            printf("the personne %d has exit\n", id + 1);
            sem_post(tiket);
            exit(0);
        }
        i++;
    }
    while (--i >= 0)
        wait(NULL);
    sem_close(tiket);
    sem_unlink("/tiket");
    printf("%d\n", some);
}