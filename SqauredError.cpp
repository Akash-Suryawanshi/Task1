#include <bits/stdc++.h>
#include <fstream>

using namespace std;

double calculateError(string y,string y1){
	ofstream ErrorFile;
	ifstream baseline;
	ifstream newline;
	baseline.open(y,ios::in);
	newline.open(y1,ios::in);
	double ans = 0.0;
	int n = 0;
	while(1){
		string ch1,ch2;
		baseline >> ch1;
		newline >> ch2;
		n++;
		if(baseline.eof() || newline.eof()) break;
		ans += pow(stod(ch1)-stod(ch2),2);
	}
	return sqrt(ans/(double)n);
}
double calculatePercentageError(string y,string y1){
	ofstream ErrorFile;
	ifstream baseline;
	ifstream newline;
	baseline.open(y,ios::in);
	newline.open(y1,ios::in);
	double ans = 0.0;
	int n = 0;
	while(1){
		string ch1,ch2;
		baseline >> ch1;
		newline >> ch2;
		n++;
		if(baseline.eof() || newline.eof()) break;
		ans += abs(stod(ch1)-stod(ch2))/stod(ch1);
	}
	return (ans*100.0)/n;
}
int main(){
	string x,y;
	cin >> x >> y;
	cout << calculateError(x,y) << '\n';
	cout << "Percentage Error : ";
	cout << calculatePercentageError(x,y) << '\n';
}
