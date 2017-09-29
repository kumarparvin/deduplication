//
//  main.cpp
//  soroco
//
//  Created by Parvin ... on 26/09/17.
//  Copyright © 2017 Parvin ... All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <queue>


#define CHUNK_SIZE 18
#define HEADER_SIZE 10


void dedup(std::string inpath, std::string outpath)
{
    std::map<std::string, int> mymap;
    std::ifstream ifile;
    std::ofstream ofile;
    int ifilesize;
    int chunk;
    char buffer[CHUNK_SIZE+1];
    std::string header;
    std::string tail;
    
    ofile.open(outpath);
    ifile.open(inpath);
    
    ifile.seekg(0,ifile.end);
    ifilesize= ifile.tellg();
    
    ifile.seekg(0,ifile.beg);
    
    
    header.assign(HEADER_SIZE,'0');
    ofile.write(header.c_str(),header.length());
    chunk=1;
    while(ifilesize)
    {
        int readbyte=(ifilesize>=CHUNK_SIZE?CHUNK_SIZE:ifilesize);

        ifile.read(buffer,CHUNK_SIZE);
        buffer[CHUNK_SIZE]=0;
        
        std::string key(buffer);
        
        if(mymap.find(key)!=mymap.end())
        {
            std::string tmpstr=std::to_string(chunk);
            tail.append(tmpstr+",");
            
            int val=mymap[key];
            std::string link(std::to_string(val)+",");
            ofile.write(link.c_str(),link.length());
        }
        else
        {
            ofile.write(buffer,readbyte);
            mymap[key]=chunk;
        }
        chunk++;
       /* if(chunk*readbyte>10000)
        {
            ofile.flush();
        }*/
        ifilesize-=readbyte;
    }

    int lentail=tail.length();
    ofile.write(tail.c_str(),lentail);
    
    std::string newheader=std::to_string(lentail);
    
    int newheader_len=newheader.length();
    
    header.replace(HEADER_SIZE-newheader_len,newheader_len,newheader);
    ofile.seekp(0);
    ofile.write(header.c_str(),header.length());
    
    ofile.close();
    ifile.close();
    
}



bool redup(std::string inpath, std::string outpath)
{
    std::queue<int> que;
    std::ifstream ifile;
    std::ofstream  ofile;
    char header_buff[HEADER_SIZE+1];
    int size;
    
    ifile.open(inpath);
    ofile.open(outpath);
    
    ifile.read(header_buff, HEADER_SIZE);
    header_buff[HEADER_SIZE]=0;
    int header=stoi(std::string(header_buff));
    
    ifile.seekg(0,ifile.end);
    size=ifile.tellg();
    
    ifile.seekg(size-header);
    
    std::string tail;
    
    int tmp_sz=header;
    while(tmp_sz--)
    {
        char ch;
        ifile.get(ch);
        if(ch==',')
        {
            que.push(std::stoi(tail));
            tail.clear();
        }
        else
        {
            tail.append(1,ch);
        }
    }
    
    
    int file_start=HEADER_SIZE;
    ifile.seekg(file_start);
    char buff[CHUNK_SIZE+1];
    int end_file=size-header;
    int chunk=1;
    while(file_start<end_file)
    {
        int read_byte=0;
        if(!que.empty() && chunk==que.front())
        {
            que.pop();
            std::string strng;
            char ch;
            while(ifile.get(ch) && ch!=',')
            {
                strng.append(1,ch);
            }
            int pos=stoi(strng);
            ofile.close();
            std::ifstream tmpfile(outpath);
            tmpfile.seekg((pos-1)*CHUNK_SIZE);
            tmpfile.read(buff,CHUNK_SIZE);
            buff[CHUNK_SIZE]=0;
            tmpfile.close();
            ofile.open(outpath,std::ios::app);
            ofile.write(buff,CHUNK_SIZE);
            read_byte=strng.length()+1;
        }
        else
        {
            read_byte=((file_start+CHUNK_SIZE<=end_file)?CHUNK_SIZE:end_file-file_start);
            ifile.read(buff,read_byte);
            buff[read_byte]=0;
            ofile.write(buff, read_byte);
            
        }
        chunk++;
        file_start+=read_byte;
        
        
    }
    ofile.close();
    ifile.close();
    
    
    return true;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    //char chunk[1024];
    //std::srand(0);
    std::ofstream file("parvin.txt");
    file<<"abcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabc232abcdefabcabc#$#@abcghabcdefghiabcabcdefabcabcabcghighijhkighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefg23223hiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhkabcdefghiabcabcdefabcabcabcghighijhk";
    file.close();
    dedup("parvin.txt", "output.txt");
    redup("output.txt","newfile.txt");

    return 0;
}
