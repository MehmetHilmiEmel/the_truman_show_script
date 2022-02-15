#include <iostream>
#include <vector>   // library to use vectors
#include <fstream> // library for reading text file
#include <string>
#include <ctime>    //library to calculate execution time
#include <cstring>
#include<algorithm>


using namespace std;

class subSentence{   // class for sentences inside the file.
public:
    string theSentence;
    vector<string>words;
    double similarityRate;
    int wordCounter;
    subSentence(){
        theSentence="";
        similarityRate=0.0;
        wordCounter=0;
    }
};

class file{     
public:
    vector<subSentence>sentences;   // The file is consist of sentences. I kept the sentences in the subSentence vector.
    
    file(fstream &file){
        string sentence;
        subSentence temp;
        string tempSentence;
        while(getline(file,sentence)){

            for( int i=0;i<sentence.size();i++){ // this for loop divides the file into sentences and insert them into the "sentences" vector.

                if(sentence[i]!='.'){
                    tempSentence+=sentence[i];
                }
                else{
                    tempSentence+='.';
                    temp.theSentence=tempSentence;
                    sentences.push_back(temp);
                    tempSentence="";
                }
            }
        }
    }// end of the file constructor.

    void wordCounter(){
        int number=0;
        for( int i=0;i < sentences.size();i++){
            number=1;
            sentences[i].wordCounter+=number;
            number=0;
        }
    } // end of the wordCounter function

    
    void findingTheKeyWords(){
        string key="";
        for( int i=0;i<sentences.size();i++){
            for( int j=0;j<sentences[i].theSentence.size();j++){
                if(sentences[i].theSentence[j]!=32 && sentences[i].theSentence[j]!=46){//If it is not space and . it will add the char to the "key" string
                    key+=sentences[i].theSentence[j];
                }
                else{
                    if(key.size()>2){
                        bool found=false;
                        for( int k=0;k<sentences[i].words.size();k++){// checking word is distinct or not
                            if(sentences[i].words[k]==key){
                                found=true;
                                break;
                            }
                        }
                        if(!found){
                            sentences[i].words.push_back(key);
                        }
                    }
                    key="";
                }
            }
        }
    }

    void quickSort(int low,int high){
        int j;
        if(low<high){
            j=partialSorting(low,high);
            quickSort(low,j-1);
            quickSort(j+1,high);

        }
    } // end of hte quick sort function

    int partialSorting(int low,int high){
        int i,j;
        double smlrty;
        smlrty=sentences[low].similarityRate;
        i=low;
        j=high+1;
        do{
            do
                i++;
            while(sentences[i].similarityRate>smlrty && i<=high);
            do
                j--;
            while(smlrty>sentences[j].similarityRate);
            if(i<j){
                swap(i,j);
            }

        }while(i<j);
        swap(low,j);

        return j;
    } // end of the partial sorting function


    void swap(int x,int y){
        subSentence temp=sentences[x];
        sentences[x]=sentences[y];
        sentences[y]=temp;
    } // end of the swap function




    void output(string question, string answer){
        string key="";
        vector<string> words;
        vector<string>stopWords;
        string line;
        fstream stopwords;
        stopwords.open("stopwords.txt",ios::in);


         for( int j=0;j<question.size();j++){
                if(question[j]!=32 && question[j]!=63){//If it is not space and . it will add the char to the "key" string
                    key+=question[j];
     
                }
                else{
                    words.push_back(key);
                    key="";
                }
                    
            }

            for (int i = 0; i < words.size(); i++)  // this loop will find the same words between question and answer and delete them
            {
                if (words[i].size()>=2)
                {                 
                    auto n = answer.find(words[i]);
                    if (n != string::npos)
                    {
                        answer.erase(n, words[i].length());
                    }
                }
            }
        while (getline(stopwords,line))
        {   

            if(line.size()>=3){
                auto n = answer.find(line);
                if (n != string::npos)
                {
                    answer.erase(n, line.length());
                }
            }    
        }
        stopwords.close();

        char punctuation [16];
        for (int i = 0; i < 16; i++)
        {
            punctuation[i]=i+32;
        }
        for (int i = 0; i < 16; i++)
        {
            answer.erase(remove(answer.begin(),answer.end(),punctuation[i]),answer.end());
        }
         cout<<answer;


    }
        
    

};

int wordCounter(string sentence){
    int number=0;
        
        for( int j=0;j < sentence.size();j++){
            if(sentence[j+1] == 32 && sentence[j] != 32){//32 is ascii value of space character
                number++;
            }
        }
        return number+1; // I added +1 because it doesn't count the last word in the sentence.
    } // end of the wordCounter function

bool HorsPool(string &sentence,string &word,int shiftTable[]){
    int m=sentence.size();
    int n=word.size();
    if(n>=m){
        return false;
    }
    int i=0;
    while(i<=m-n){
        for(int j=n-1;j>=0;j--){
            if(word[j]!=sentence[i+j]){
                break;

            }
            if(j==0){
                return true;
            }
        }
        i=i+n-1;
        i=i+shiftTable[(int)sentence[i]]; //according to the word's shift table, it will shift.
        i=i-n+1;
    }
    return false;

} // end of the horspool function

void sentenceComparison(string questionSentence,file &trumanShow){
    int counter=0;
    double similarity =0.0;
    
    int trumanSize=trumanShow.sentences.size();
    for(int i=0;i<trumanSize;i++){
        int x= trumanShow.sentences[i].words.size();
        int (*shiftTable)[128] {new int[x][128]};// There are 128 character on the ASCII table
         for(int j=0;j<x;j++){//this for loop finds the bad char tables for every words.
            for(int k=0;k<128;k++){
                shiftTable[j][k]=trumanShow.sentences[i].words[j].size();
            }
            for( int k=0;k<trumanShow.sentences[i].words[j].size()-1;k++){
                shiftTable[j][(int)trumanShow.sentences[i].words[j][k]]=trumanShow.sentences[i].words[j].size()-k-1; 
                // this for loop crate shifting table for the words
                // for example, let's say our word is Truman. T[5],r[4],u[3],m[2],a[1] the other ascii values are 6.
            }
        }
            for(int k=0;k<x;k++){
                if(HorsPool(questionSentence,trumanShow.sentences[i].words[k],shiftTable[k])){
                    counter++; // when horspool function finds match, counter variable will be incremented by one.
                }
            }
            similarity=((double(counter)/double(trumanShow.sentences[i].wordCounter))*100.0); //this line calculates the similarity rate
            trumanShow.sentences[i].similarityRate=similarity;
            counter=0; //this line resets the counter for new sentences
            similarity=0.0;// this line resets the similarity for new sentences
    }
} // end of the sentence comparison function.







int main()
{   clock_t start;//this is for execution time
    double time;
    start=clock();

    // reading the_truman_show_script file
    fstream trumanFile;
    trumanFile.open("the_truman_show_script.txt",ios::in);
    file trumanShow(trumanFile);
    
    trumanShow.wordCounter();
    trumanShow.findingTheKeyWords();

    // reading questions.txt file
    fstream mainFile;
    mainFile.open("questions.txt",ios::in);
    string questionSentence;
     
    int i=0;

    while (getline(mainFile,questionSentence))
   {       
    
        std::cout<<"question "<<i+1<<" :"<<questionSentence<<endl;    //this line shows the sentences from questions.txt
        sentenceComparison(questionSentence,trumanShow);    //this line compares questions.txt sentences with the_truman_show_script.txt sentences 
        trumanShow.quickSort(0,trumanShow.sentences.size()-1);  //this line is sorting the similarities of the sentences
        //std::cout<<"answer:"<<trumanShow.sentences[0].theSentence<<endl<<endl;  //this line prints the most similar sentence to the console
        cout<<"Answer: {";
        trumanShow.output(questionSentence,trumanShow.sentences[0].theSentence);
        cout<<"}"<<endl;
        cout<<endl;
        
        
        i++;
    }

    trumanFile.close();
    mainFile.close();  
    time=(clock()-start)/(double)CLOCKS_PER_SEC;
    std::cout<<endl<<"execution time = "<<time<<" s"<<endl;

    return 0;
}