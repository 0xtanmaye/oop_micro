#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(){
    
    int choice;
    cout<<"menu:\n1.Register\n2.Login"<<endl;
    cin>>choice;

    string username, password, un, up;

    if (choice==1){
        cout<<"Enter Username To Register with: ";
        cin>>username;

        ifstream acc;
        acc.open("database\\"+username+".txt");
        if (acc)
        {
            cout<<"Member Already Exist!";
            main();
        }
        else
        {
            cout<<"Create a Password: ";
            cin>>password;

            ofstream creds;
            creds.open("database\\"+username+".txt");
            if (!creds)
            {
                cout<<"Error Creating Account! Please try again!"<<endl;
            }
            else
            {
                creds << username << endl << password << endl;
                cout<<"Registered Successfully!" << endl;
                creds.close();
                main();
            }
        }
    }

    else if (choice==2)
    {
        cout<<"Enter Username: ";
        cin>>username;

        ifstream locate;
        locate.open("database\\"+username+".txt");
        if (!locate)
        {
            cout<<"You are not an member, register first!";
            main();
        }
        else
        {
            cout<<"Enter Password for "<<username<<" : ";
            cin>>password;

            ifstream user;
            user.open("database\\"+ username + ".txt");
            getline(user, un);
            getline(user, up);
            if (un==username && up==password)
            {
                cout<<"Successfully Logged in..!";
            }
            else
            {
                cout<<"password is wrong for username";
                main();
            }
            
        }
        
    }
    return 0;
}