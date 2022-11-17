#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <cctype>
#include "picosha2.h"
#include "timer.h"

using namespace std;

// Threads are turned off because no improvement in performance was detected.
// This could be because of how the program works or caused by a terrible implementation (or both)
//#define THREAD

namespace Xor{
    void Transform(string& input, string key)
    {
        if(!key.size())
            return;

        // Key hashed in sha256, makes for a bigger, more random -> better encryption
        // I think ?
        key = picosha2::hash256_hex_string(key);

        for (auto i = 0; i < input.length(); i++)
            input[i] ^= key[i%key.size()];
    }

    int xorFile(const string& path, const string& key)
    {
        string output;
        ifstream file(path);
        if (file.is_open())
        {
            string buff;
            while (getline(file, buff))
                output += (buff + '\n');

            file.close();

#ifdef THREAD

            thread th(Transform, ref(output), key);
            th.join();
#else
            Transform(output, key);
#endif

            ofstream file(path);
            file.clear();
            if (file.is_open())
            {
                file.write(output.c_str(), output.length() - 1); // - 1 to avoid printing that weird char at the end
                file.close();
            }

            cout << "Successfully xored file with key \"" << key << "\"" << endl;
        }
        else
        {
            cerr << "Couldn't open the file !" << endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    bool cmp(pair<char, float>& a, pair<char, float>& b)
    {
        return a.second > b.second;
    }

    void Analyze(string cipher)
    {
        vector<pair<char, float>> freq;
        transform(cipher.cbegin(), cipher.cend(), cipher.begin(), [](char c){return toupper(c);});

        for (int i = 'A'; i <= 'Z'; i++)
            freq.push_back(make_pair<char, float>(char(i), (float)count(cipher.begin(), cipher.end(),char(i)) / (float)cipher.length()));

        sort(freq.begin(), freq.end(), cmp);
        cout << "\nFrequency analysis for this cihper: " << endl;

        for (auto [key, value] : freq)
            cout << key << ": " << value*100.f << "% ";

        cout << endl;
    }

    int analyzeFile(const string& path)
    {
        string output;
        ifstream file(path);
        if (file.is_open())
        {
            string buff;
            while (getline(file, buff))
                output += (buff + '\n');
            file.close();

#ifdef THREAD
            thread th(Xor::Analyze, output);
            th.join();
#else
            Xor::Analyze(output);
#endif
        }
        else
        {
            cerr << "Couldn't open the file !" << endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
};


int main(int argc, char** argv)
{
    string path, key;

    if (argc == 3)
    {
        path = argv[1];
        key = argv[2];

#ifdef THREAD
            thread th(Xor::xorFile, path, key);
            th.join();
#else
            Xor::xorFile(path, key);
#endif
    }
    else if (argc == 2)
    {
        path = argv[1];

#ifdef THREAD
            thread th(Xor::analyzeFile, path);
            th.join();
#else
            Xor::analyzeFile(path);

#endif
    }
    else
    {
        cerr << "Wrong arguments !\nSupported modes: \n\tfrequency analysis: ./SimpeXorEncrypter {file} \n\txor: ./SimpeXorEncrypter {file} {key}" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}