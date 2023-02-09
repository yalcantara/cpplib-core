#pragma once

#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <vector>
#include <experimental/filesystem>

#include <ylib/core/sutils.h>

using ylib::core::endsWith;

namespace ylib {
namespace io {
        
    typedef std::basic_ofstream<char16_t> u16ofstream;



    vector<string> listFiles(const string& path){
        namespace fs = std::experimental::filesystem;

        vector<string> ans;
        for (fs::directory_entry entry : fs::directory_iterator(path)){
            ans.push_back(entry.path());
        }

        return ans;
    }


    void write(const string& path, const vector<unsigned char>& data) {
        ofstream myFile (path, ios::out | ios::binary);
        const unsigned char* buffer = data.data();
        myFile.write ((char*)buffer, data.size());
        myFile.close();
    }

    void write(const string& path, string& content) {
        FILE* file = fopen(path.c_str(), "w");
        if (file == NULL) {
            throw Exception(sfput("Could not open file: '${}'.", path));
        }

        
        fprintf(file, "%s", content.c_str());
        fflush(file);
        fclose(file);
    }

    void writeln(const char* dir, const char* fileName, const char* mode, const char* content) {

        string full;

        string sdir{ dir };
        string sfileName{ fileName };
        if (endsWith(sdir, '/') == True) {
            full = sdir + fileName;
        } else {
            full = sdir + "/" + fileName;
        }

        struct stat sb;
        if (stat(dir, &sb) != 0 || S_ISDIR(sb.st_mode) == false) {

            if (mkdir(dir, 0777) != 0) {
                throw Exception(sfput("The directory '${}' does not exist and could not be created.", dir));
            }

            
        }

        FILE* file = fopen(full.c_str(), mode);
        if (file == NULL) {
            throw Exception(sfput("Could not open file: '${}'.", full));
        }

        fprintf(file, "%s", content);
        fputc('\n', file);
        fflush(file);
        fclose(file);
    }

    void writeln(const char* dir, const char* fileName, const char* mode, const u16string& content) {

        string full;

        string sdir{ dir };
        string sfileName{ fileName };
        if (endsWith(sdir, '/') == True) {
            full = sdir + fileName;
        } else {
            full = sdir + "/" + fileName;
        }

        struct stat sb;
        if (stat(dir, &sb) != 0 || S_ISDIR(sb.st_mode) == false) {

            if (mkdir(dir, 0777) != 0) {
                throw Exception(sfput("The directory '${}' does not exist and could not be created.", dir));
            }

            
        }

        FILE* file = fopen(full.c_str(), mode);
        if (file == NULL) {
            throw Exception(sfput("Could not open file: '${}'.", full));
        }
        fclose(file);

        u16ofstream outfile(full.c_str(), std::ios_base::app);
        outfile << content; 
        outfile.close();
    }

    void writeln(string& dir, string& fileName, const char* mode, string& content) {
        writeln(dir.c_str(), fileName.c_str(), mode, content.c_str());
    }

    void writeln(const char* dir, const char* fileName, const char* mode, const string& content){
        writeln(dir, fileName, mode, content.c_str());
    }

    void writeln(const char* dir, string& fileName, const char* mode, string& content) {
        writeln(dir, fileName.c_str(), mode, content.c_str());
    }

    void write(string&& path, string& content) {
        write(path, content);
    }


    string ffull(const char* path){


        FILE *f = fopen(path, "rb");
        if (f == NULL) {
            string spath = path;
            string msg = "Could not open file: '" + spath +"'.";
            throw Exception(msg);
        }

        
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        char* txt = (char*)malloc(fsize + 1);
        size_t totalRead = fread(txt, 1, fsize, f);
        fclose(f);

        if(totalRead != fsize){
            stringstream ss;
            ss << "Error while reading file: '";
            ss << path;
            ss << "'.";
            throw Exception(ss.str());
        }

        txt[fsize] = 0;

        string ans = txt;
        free(txt);

        return ans;
    }



}
}