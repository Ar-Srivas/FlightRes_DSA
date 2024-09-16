#include <iostream>
#include "Seat_matrix.cpp"
using namespace std;

int main()
{   Seat_mat s1;
    s1.default_Seat_Matrix();
    s1.display_seat_Matrix();
    int number_tickets=0;
    int pos_i=0,pos_j=0;
    cout<<"Enter number of tickets you want to book"<<endl;
    cin>>number_tickets;
    for(int i =1;i<=number_tickets;i++)
    {
        cout<<"Enter i and j for seat"<<endl;
        cin>>pos_i>>pos_j;
        s1.book_seat(pos_i,pos_j);
        if(s1.count_seat==s1.m*s1.n)
        {
        cout<<"full"<<endl;
        break;
        }
    }
    s1.display_seat_Matrix();
    return 0;
}
