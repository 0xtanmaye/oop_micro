#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <tchar.h>
#include <Windows.h>
#include <io.h>
#define SIZE 1000
using namespace std;

const string ADMIN_USERNAME = "admin";

void clear_screen()
{
    #ifdef lx
	system("clear");
    #else
	system("cls");
    #endif
}

bool ChCrDir() {
  string dirname = "database";
  struct stat st;

  if (stat(dirname.c_str(), &st) == 0) {
    // Directory exists
    if (st.st_mode & S_IFDIR) {
        //most likely to be a folder
        return true;
    }
  } else {
    // Directory does not exist

    #ifdef lx
        // string dirname = "database";
        system("mkdir database");
        return true;
    #else
        system("md database");
        return true;
    #endif
    }

}

char* enc_b64(string input_str, int len_str)
{
   
    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      
    char *res_str = (char *) malloc(SIZE * sizeof(char));
      
    int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
    int i, j, k = 0;
      
    for (i = 0; i < len_str; i += 3)
        {
            val = 0, count = 0, no_of_bits = 0;
  
            for (j = i; j < len_str && j <= i + 2; j++)
            {

                val = val << 8;

                val = val | input_str[j];

                count++;
              
            }
  
            no_of_bits = count * 8;
  
            padding = no_of_bits % 3;
  

            while (no_of_bits != 0)
            {
                if (no_of_bits >= 6)
                {
                    temp = no_of_bits - 6;
                      
                    index = (val >> temp) & 63;
                    no_of_bits -= 6;        
                }
                else
                {
                    temp = 6 - no_of_bits;
                      
                    index = (val << temp) & 63;
                    no_of_bits = 0;
                }
                res_str[k++] = char_set[index];
            }
    }

    for (i = 1; i <= padding; i++)
    {
        res_str[k++] = '=';
    }
  
    res_str[k] = '\0';
  
    return res_str;
  
}


char* dc_b64(string encoded, int len_str)
{
    char* decoded_string;
 
    decoded_string = (char*)malloc(sizeof(char) * SIZE);
 
    int i, j, k = 0;
 
    int num = 0;
 

    int count_bits = 0;
 
    for (i = 0; i < len_str; i += 4)
    {
        num = 0, count_bits = 0;
        for (j = 0; j < 4; j++)
        {
             
            if (encoded[i + j] != '=')
            {
                num = num << 6;
                count_bits += 6;
            }
 
            if (encoded[i + j] >= 'A' && encoded[i + j] <= 'Z')
                num = num | (encoded[i + j] - 'A');

            else if (encoded[i + j] >= 'a' && encoded[i + j] <= 'z')
                num = num | (encoded[i + j] - 'a' + 26);
 
            else if (encoded[i + j] >= '0' && encoded[i + j] <= '9')
                num = num | (encoded[i + j] - '0' + 52);
 
            else if (encoded[i + j] == '+')
                num = num | 62;
 
            else if (encoded[i + j] == '/')
                num = num | 63;

            else {
                num = num >> 2;
                count_bits -= 2;
            }
        }
 
        while (count_bits != 0)
        {
            count_bits -= 8;

            decoded_string[k++] = (num >> count_bits) & 255;
        }
    }

    decoded_string[k] = '\0';
 
    return decoded_string;
}


int adminFunctions(){
    clear_screen();
    int adminC;
    cout<<"!! Admin Panel !!"<<endl;
    cout<<"Menu:\n1. View All Users.\n2. Delete an User.\n3. Add an User.\n4. Exit \n:";
    cin>>adminC;
    if(adminC==1){
        string dir_path = "database\\";

        HANDLE handle;
        WIN32_FIND_DATA data;
        handle = FindFirstFile((dir_path + "*").c_str(), &data);
        if (handle == INVALID_HANDLE_VALUE) {
            cerr << "Error: No files found in the directory" << endl;
            return -1;
        }
        // cout << data.cFileName << endl;
        int i = 1;
        while (FindNextFile(handle, &data)) {
            // Skip the first two files
            if (i++ < 2) continue;
            cout << data.cFileName << endl;
        }
        FindClose(handle);  
    }
    else if (adminC==2)
    {
        string temp_un;

        cout<<"Enter the username to delete: ";
        cin>>temp_un;
        string dir_path = "database\\";

        // Check if the file exists in the directory
        DWORD file_attributes = GetFileAttributes((dir_path + temp_un).c_str());
        if (file_attributes == INVALID_FILE_ATTRIBUTES) {
            cout << "The User '" << temp_un << "' does not exist!" << endl;
        } else{
                if (DeleteFile((dir_path + temp_un).c_str())) {
                    cout << "The user '" << temp_un << "' was successfully deleted" << endl;
                    adminFunctions();
                } else {
                    cerr << "Error: The user '" << temp_un << "' could not be deleted" << endl;
                    adminFunctions();
                }
        } 
    }
    else if(adminC==3){
        string username, password;
        cout<<"Enter Username To Register with: ";
        cin>>username;

        ifstream acc;
        acc.open("database\\"+username);
        if (acc)
        {
            cout<<"Member Already Exist!";
            adminFunctions();
        }
        else
        {
            cout<<"Create a Password: ";
            cin>>password;

            ofstream creds;
            creds.open("database\\"+username);
            if (!creds)
            {
                cout<<"Error Creating Account! Please try again!"<<endl;
            }
            else
            {
                // int un_size = username.length()+1;
                // char cr_arr[un_size];
                string enc_un = enc_b64(username, username.length());
                string enc_up = enc_b64(password, password.length());
                creds <<  enc_un << endl << enc_up << endl;
                cout<<"Registered Successfully!" << endl;
                creds.close();
                Sleep(3000);
                adminFunctions();
            }
        }
    }
}


int main(){
    if(ChCrDir()){
        string LoggedInUser;
        int choice;
        cout<<"menu:\n1.Register\n2.Login\n3.Exit"<<endl;
        cin>>choice;

        string username, password, un, up;
        
        if (choice==1){
            clear_screen();
            cout<<"Enter Username To Register with: ";
            cin>>username;

            ifstream acc;
            acc.open("database\\"+username);
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
                creds.open("database\\"+username);
                if (!creds)
                {
                    cout<<"Error Creating Account! Please try again!"<<endl;
                }
                else
                {
                    // int un_size = username.length()+1;
                    // char cr_arr[un_size];
                    string enc_un = enc_b64(username, username.length());
                    string enc_up = enc_b64(password, password.length());
                    creds <<  enc_un << endl << enc_up << endl;
                    cout<<"Registered Successfully!" << endl;
                    creds.close();
                    main();
                }
            }
        }

        else if (choice==2)
        {
            clear_screen();
            cout<<"Enter Username: ";
            cin>>username;

            ifstream locate;
            locate.open("database\\"+username);
            if (!locate)
            {
                cout<<"You are not an member, register first!";
                exit(1);
            }
            else
            {
                cout<<"Enter Password for "<<username<<" : ";
                cin>>password;

                ifstream user;
                user.open("database\\"+ username);
                getline(user, un);
                getline(user, up);

                // int un_size = un.length()+1;
                // int up_size = up.length()+1;

                if (username==dc_b64(un, un.length()) && password==dc_b64(up, up.length()))
                {
                    cout<<"Successfully Logged in..!";
                    LoggedInUser = username;
                    if(LoggedInUser == ADMIN_USERNAME) {
                        adminFunctions();
                    }

                }
                else
                {
                    cout<<"password is wrong for username";
                    exit(1);
                }
                
            }
            
        }
    }
    else
    {
        cout<<"ughh... Some Totaly unexpected error occured .. man .. shit!";
        exit(1);
    }
    
    return 0;
}