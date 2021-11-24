#include <bits/stdc++.h>
using namespace std;
map<string, string>defined;
map<string, bool>isDataType;
vector<string>dataType{"int", "long long", "char", "string", "bool", "return"};
string substring(string s, int start, int len){
	char t[len+1];
	for(int i=0; i<len; i++)
		t[i]=s[i+start];
	t[len] = '\0';
	return t;
}
string substring(string s, int start){
	int len = s.size()-start;
	char t[len+1];
	for(int i=0; i<len; i++)
		t[i]=s[i+start];
	t[len] = '\0';
	return t;
}
string add(string a, string b){
	int l1=a.size(), l2=b.size();
	char t[l1+l2+1];
	for(int i=0; i<l1; i++)
		t[i] = a[i];
	for(int i=0; i<l2; i++)
		t[i+l1] = b[i];
	t[l1+l2]='\0';
	return t;
}
string add(string a, string b, string c){
	return add(a, add(b, c));
}
bool compare(string a, int start, string b){
	int len = b.size();
	for(int i=0; i<len; i++){
		if(a[i+start]!=b[i])
			return false;
	}
	return true;
}
void applyDefine(string& s){
	for(auto pp:defined){
		string from = pp.first, to = pp.second;
		int len = from.size(), n=s.size();
		for(int i=n-len; i>=0; i--){
			if(substring(s, i, len)==from)
				s=add(substring(s, 0, i), to, substring(s, i+len));
		}
	}
}
void indent(int n){
	while(n--)
		cout<<"\t";
}
int main(){
	ios_base::sync_with_stdio(false);cin.tie(NULL);
	for(auto x:dataType){
		isDataType[x]=true;
		isDataType[x+"[]"] = true;
	}
	int function = 1, openBracket = 0;
	bool isMain = false, comment = false;
	int tabs=0;
	stack<int>curlyBrackets;
	bool toIndent = false;
	while(1){
		string s;
		getline(cin, s);
		if(s=="")
			continue;
		if(compare(s, 0, "#include")) continue;
		applyDefine(s);
		stringstream ss(s);
		vector<string>vs;
		string t;
		while(ss>>t){
			vs.push_back(t);
		}
		if(compare(vs[0], 0, "#define")){
			string str = vs[2];
			for(int i=3; i<vs.size(); i++)
				str=add(str, add(" ", vs[i]));
			defined[vs[1]] = str;
			continue;
		}
		for(auto yy:vs){
			string y = yy;
			for(int i=0; i<y.size(); i++){
				char x = y[i];
				if(x=='/' && i!=y.size()-1 && y[i+1]=='*')
					comment = true;
				if(comment){
					if(x=='*' && i!=y.size()-1 && y[i+1]=='/')
						{
							comment = false;
							i++;
						}
					continue;
				}
				if(x=='/' && i!=y.size()-1 && y[i+1]=='/')
					break;
				if(x=='}'){
					tabs--;
					curlyBrackets.pop();
				}
				if(x=='{'){
					curlyBrackets.push(tabs);
					tabs--;
				}
				if(toIndent) {
					indent(tabs);
					toIndent = false;
				}
				cout<<x;
				if(x==';'){
					cout<<"\n";
					toIndent = true;
					y=substring(y, i+1);
					i=-1;
					if(curlyBrackets.empty())
						function++;
					else if(tabs != curlyBrackets.top())
						tabs --;
				}
				if(x=='{'){
					cout<<"\n";
					tabs++;
					toIndent = true;
					y=substring(y, i+1);
					i=-1;
				}
				if(x=='}'){
					if(!tabs && isMain){
						function--;
						if(!function)
							return 0;
					}
					cout<<"\n";
					toIndent = true;
					y=substring(y, i+1);
					i=-1;
				}
				if(x=='('){
					if(i==4 && substring(y, 0, 5)=="main(")
						isMain = true;
					openBracket++;
					y=substring(y, i+1);
					i=-1;
				}
				if(x==')'){
					openBracket--;
					if(!openBracket){
						if(i==y.size()-1 || y[i+1]!=';'){
							tabs++;
							cout<<"\n";
							toIndent = true;
						}
					}
					y=substring(y, i+1);
					i=-1;
				}
				if(i==3 && (y.size()==4 || (y.size()>4 && y[4]=='{') ) && substring(y, 0, 4)=="else"){
					tabs++;
					cout<<"\n";
					toIndent = true;
				}
			}
			if(isDataType[y])
				cout<<" ";
		}
	}
}
