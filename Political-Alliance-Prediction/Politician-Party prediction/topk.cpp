#include<iostream>
#include<fstream>
#include<cstring>
#include<bits/stdc++.h>

#define parties 5
#define K 7
#define T 7
using namespace std;

typedef pair<double,string> pi;
/**
 * Reads csv file into table, exported as a vector of vector of doubles.
 * @param inputFileName input file name (full path).
 * @return data as vector of vector of doubles.
 */
vector<vector<string>> parse2DCsvFile(string inputFileName) {

    vector<vector<string> > data;
    ifstream inputFile(inputFileName);
    int l = 0;

    while (inputFile) {
        l++;
        string s;
        if (!getline(inputFile, s)) break;
        if (s[0] != '#') {
            istringstream ss(s);
            vector<string> record;

            while (ss) {
                string line;
                if (!getline(ss, line, ','))
                    break;
                try {
                    record.push_back((line));
                }
                catch (const std::invalid_argument e) {
                    cout << "NaN found in file " << inputFileName << " line " << l
                         << endl;
                    e.what();
                }
            }

            data.push_back(record);
        }
    }

    if (!inputFile.eof()) {
        cerr << "Could not read file " << inputFileName << "\n";
        __throw_invalid_argument("File not found.");
    }

    return data;
}


bool sort_by_indegree(const vector<string> &left,const vector<string> &right)
{
    return stoi(left[2]) > stoi(right[2]);
}


int main()
{


    vector<vector<string>> data = parse2DCsvFile("topkhelperfile.csv"); //stores politician,party,weighted_indegree.
    unordered_map<string,vector<string>> top_k_pol;                     //stores top k politicians of party.
    unordered_map<string,string> poli_party;                            //Maps politician to party.

    unordered_map<string,int> counter;
    cout<<"Size of data is: "data.size()<<"\n";
    sort(data.begin(),data.end(),sort_by_indegree);                     //Sorting based on indegree to find top k influential nodes.
    for(int i=0;i<data.size();i++)
    {
        poli_party[data[i][0]]=data[i][1];                              //Mapping politician to party.
        
        if(counter.count(data[i][1])!=0 && counter[data[i][1]]>=K)
            continue;
        top_k_pol[data[i][1]].push_back(data[i][0]);
        counter[data[i][1]]++;
    }

    //Find average sentiment of all the politician
    //IF politician is in top k or it has never interacted with party then skip.
    //Otherwise compare him with sentiment of top k and find closest T party
    //check with his original party if it matches increase counter
    //find accuracy = counter/total.

    //Reading edges file to find avg sentiment of each politician.
    vector<vector<string>> data_edges=parse2DCsvFile("all_edges.csv");
    unordered_map<string, pair<double, double>> senti;      //politician, sentiment,total edges.
    unordered_map<string,unordered_set<string>> poli_party_interact;    //stores parties which politician has interacted.

    for (int i = 1; i < data_edges.size(); i++) {
        string source = data_edges[i][0], target = data_edges[i][1], sentiment = data_edges[i][2];
        if (sentiment == "2")
            senti[source].first++, senti[target].first++;
        else if (sentiment == "1")
            senti[source].first--, senti[target].first--;
        senti[source].second++, senti[target].second++;
        string p2=poli_party[target];
        poli_party_interact[source].insert(p2);
    }

    //Now for each politician we will check similarity of all topk politicians of party and collect topT politicians.
    double accuracy=0.0;
    for(int i=0;i<data.size();i++)
    {
        if(poli_party_interact.count(data[i][0])==0)   continue;
        double senti_poli=(double)(senti[data[i][0]].first/senti[data[i][0]].second);
        priority_queue<pi,vector<pi>, greater<pi>> top_t_parties;
        for(auto m:top_k_pol)
        {
            //we need to check if there is interaction.
            if(poli_party_interact[data[i][0]].find(m.first)==poli_party_interact[data[i][0]].end())
            {
                continue;
            }
            vector<string> tempk=m.second;
            for(int j=0;j<tempk.size();j++)
            {
                double k=(double)(senti[tempk[j]].first/senti[tempk[j]].second);
                double sim=(senti_poli-k)*(senti_poli-k);                           //Similarity is found using min square error
                top_t_parties.push(make_pair(sim,m.first));
            }
        }
        map<string,double> mp;
        pair<double,string> best=top_t_parties.top();
        for(int j=0;j<T;j++)
        {
            if(top_t_parties.empty()) break;
            mp[top_t_parties.top().second]++;
            top_t_parties.pop();
        }
        bool flag=false;
        string party_predicted;
        int max_found=INT_MIN;
        for(auto m:mp)
        {
            if(max_found < m.second)
            {
                max_found=m.second;
                flag=true;
                party_predicted=m.first;
            }
            else if(max_found == m.second)
                flag=false;
        }
        if(!flag)
        {
            //party_predicted=data[i][1];
            party_predicted=best.second;
        }
        if(party_predicted==data[i][1]) accuracy++;
        else{
            cout<<"Original Party for politician: "<<data[i][0]<<" is: "<<data[i][1]<<" and predicted is: "<<party_predicted<<"\n";
        }
    }
    cout<<poli_party_interact.size() <<endl;
    cout<<"Accuracy is: "<<accuracy/poli_party_interact.size();
    return 0;
}