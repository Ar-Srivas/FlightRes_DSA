#include <iostream>
using namespace std;
class Seat_mat
{
public:
    int m=10;
    int n=10;
    int seat[10][10];
    int count_seat=0;//Counts the number of seats not booked
    void default_Seat_Matrix()
    {
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                seat[i][j]=0;
            }

        }
    }
    int count_Seat_matrix()
    {
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(seat[i][j]==1)
                count_seat++;

            }

        }
        return count_seat;
    }
    void display_seat_Matrix()
    {
      for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                cout<<seat[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    void book_seat()
    {
    if (count_seat == m * n) {
        cout << "Flight is full" << endl;
        return;
    }

    for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < n; j++)
       {
         if (seat[i][j] == 0)
         {
            seat[i][j] = 1;
            count_seat++;
            return;
                }
            }
        }
    }
};

