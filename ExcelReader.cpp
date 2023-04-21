////
//// Created by caleb on 4/20/2023.
////
//
//#include <iostream>
//#include <vector>
//#include <fstream>
//#include <sstream>
//using namespace std;
//
//vector<string> ExcelReader(string fileName);
//
//vector<string> ExcelReader(string fileName) {
//    string path = "C:/Users/caleb/" + fileName + ".csv";
//    ifstream storeFile(path);
//
//    while (true) {
//        if (!storeFile.is_open()) {
//            cerr << "Error opening the file:" << path << endl;
//        } else {
//            string line;
//            while(getline(storeFile, line)) {
//                istringstream line_stream(line);
//                string cell;
//
//                while (getline(line_stream, cell, ',')) {
//                    cout << cell << "\t"
//                }
//                cout << endl;
//            }
//            storeFile.close();
//        }
//    }
//}
