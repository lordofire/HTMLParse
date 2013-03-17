#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <curl/curl.h> //your directory may be different
using namespace std;

vector<string> split(const string& line) {
    int j = 0, k = 0;
    vector<string> arg;
    for (vector<string>::size_type i = 0; i != line.length(); ++i)
    {
        if (line.at(i) == ' ')
        {
            arg.push_back(line.substr(j,k));
            j = i+1;
            k = 0;
        } else if (i == line.length()-1) {
            arg.push_back(line.substr(j));
        } else {
            k++;
        }
    }
    return arg;
}

void Sub_detect(string &line){
    while(1){
        std::string::size_type foundbegin, foundend;
        foundbegin = line.find("<");
        if(foundbegin == std::string::npos)
            break;
        else
            foundend = line.find(">");        
        line.erase(foundbegin, foundend+1);        
    }
}

void InfoExtract(string in_filename){

    string line, arg1;
    vector<string> arg;
    std::string::size_type found;

    ifstream inputfile;
    inputfile.open(in_filename.c_str());

    ofstream out_Pfile, out_LIfile, out_WPfile;     
    out_Pfile.open("ParagraphResult.txt");
    out_LIfile.open("listitemResult.txt");
    out_WPfile.open("WebPageResult.txt");

    if(!inputfile.is_open()){
        cout<< "The file " << in_filename << " fails to be opened!" <<endl;
        return;
    }

    while(1){
        if(inputfile.eof())
            break;
        else{
            getline(inputfile, line);
        }

        arg = split(line);

        if((found = line.find("<p")) != std::string::npos && (found = line.find("http:")) != std::string::npos && line.find(".html") != std::string::npos){
            line = line.substr(found);
            found = line.find(".html");
            line.erase(found + 5, line.size());
            if(line.c_str()){
                out_WPfile<<line<<endl;
                cout<<"HTML: "<<line<<endl;
            }
        }else if ((found = line.find("<p")) != std::string::npos) {
            line = line.substr(found);
            Sub_detect(line);
            if(line.c_str()){
                out_Pfile<<line<<endl;
                cout<<"P: "<<line<<endl;
            }
        }else if((found = line.find("<p>")) != std::string::npos){
            line = line.substr(found);
            Sub_detect(line);
            if(line.c_str()){
                out_Pfile<<line<<endl;
                cout<<"P: "<<line<<endl;
            }
        }else if((found = line.find("<li>")) != std::string::npos){
            line = line.substr(found);
            Sub_detect(line);
            if(line.c_str()){
                out_LIfile<<line<<endl;
                cout<<"LI: "<<line<<endl;
            }
        }
    }
}


int main(int argc, char* argv[])
{
    if(argc != 2)
        cout<<"The number of input argument is not sufficient!"<<endl;
    else{
        string url(argv[1]);
        string filename = "output.txt";

        CURL *curl;  
        CURLcode result; 
        FILE *my_file;

        curl = curl_easy_init();

        if(curl){
            my_file = fopen(filename.c_str(), "wb");
            // Set up curl
//            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            //set default fwrite
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL); 
            //write the data to the file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, my_file);
            //show the verbose information of programming
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
            //show the parsing progress
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);


            result = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            curl_global_cleanup();

            if (result == CURLE_OK)
                cout << "Success"<<endl;

            curl_global_cleanup();
        }


        //Extract the info
        InfoExtract(filename);     

        fclose(my_file);
//        remove(filename.c_str());  
    }
    return 0;
}
