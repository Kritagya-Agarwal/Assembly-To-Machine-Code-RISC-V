#include<bits/stdc++.h>
#define ll long long int
#define START 268435456
using namespace std;
vector<string>codeinit;
vector<string>code;
vector<string>Format;
ll sizeI,size;
int binary[32];
ll pccount = 0;

typedef struct 
{
	string s;
	int index;
}lab;

vector<lab>Label;

string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

struct datafile
{
    string name;
    string type;
    vector<string> value;
};

struct seg
{
    string name;
    ll position;
};

//To get 2's Complement Representation for Immediate Values
ll getinver(ll imme,int bit)
{
	ll imcopy=-imme;
	vector<int>bb;
	imme=-imme;
	for(int i=0;i<bit;i++)
	{
		bb.push_back(imme%2);
		imme/=2;
	}
	reverse(bb.begin(),bb.end());
	for(int i=0;i<bit;i++)
	{
		if(bb[i])
			bb[i]=0;
		else
			bb[i]=1;
	}
	int j=bit-1;
	while(1)
	{
		if(bb[j]==0)
		{
			bb[j]=1;
			break;
		}
		else{
			bb[j]=0;
		}
		j--;
	}
	ll num=0;
	ll mul=1;
	for(int i=bit-1;i>=0;i--)
	{
		num += bb[i]*mul;
		mul*=2;
	}
	return num;
}

vector<seg>datalabel;

//To convert a number in string format to its Hexadecimal
string convert(string s,int len)
{
    int length = s.size();
    int flag = 1;
    for(int i=0;i<length;i++)
    {
        if(s[i]=='x' || s[i]=='X')
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        ll num = 0;
        ll mul = 1;
        int last = 0;
        int flag1 = 0;
        if(s[0]=='-')		flag1 = 1;
        if(flag1)		last = 1;
        for(int i = length-1;i>=last;i--)
        {
            num += (s[i]-48)*mul;
            mul*=10;
        }

        if(flag1)
        {
        	num = getinver(num,len*4);
        }

        string ans;
        for(int i=0;i<len;i++)
        {
            ll rem = num%16;
            if(rem<=9)
            {
                ans += (rem+48);
            }
            else
            {
                ans += (rem-10+65);
            }
            num/=16;
        }
        reverse(ans.begin(),ans.end());
        return ans;
    }

    else
    {
        ll x; 
        stringstream ss; 
        ss << std::hex << s; 
        ss >> x; 
        ll num = x;
        if(num<0)
        {
        	num = getinver(num,len*4);
        }
        string ans;
        for(int i=0;i<len;i++)
        {
            ll rem = num%16;
            if(rem<=9)
            {
                ans += (rem+48);
            }
            else
            {
                ans += (rem-10+65);
            }
            num/=16;
        }
        reverse(ans.begin(),ans.end());
        return ans;
    }
}

void read_data()
{
    ifstream file;
    string word;
    vector<datafile> stored;
    file.open("test.asm");
    int flag;
    int start = 0;
    if(file.is_open())
    {
        while(!file.eof())
        {
            file >> word;
            if(start > 1)
            {
                continue;
            }
            else if(start == 1)
            {
                if(word == ".text")         //data part ends
                    start = 2;
                else
                {
                    datafile temp;
                    flag = 0;
                    int index;
                    for(int i = 0; i < word.size()-1; i++)
                    {
                        if(word[i] == ':' && word[i+1] == '.')
                        {
                            flag = 1;
                            index = i;
                        }
                    }
                    if(flag == 1)
                    {
                        string nameT = "\0", typeT = "\0";
                        for(int i = 0; i < index; i++)
                            nameT+=word[i];
                        for(int i = index+2; i < word.size(); i++)
                            typeT+=word[i];
                        temp.name = nameT;
                        temp.type = typeT;
                        getline(file, word);
                        stringstream ss(word);
                        while(ss >> word)
                        {
                            temp.value.push_back(word);
                        }
                    }
                    else
                    {
                        word.erase(word.end()-1);
                        temp.name = word;
                        file >> word;
                        word.erase(word.begin());
                        temp.type = word;
                        getline(file, word);
                        stringstream ss(word);
                        while(ss >> word)
                        {
                            temp.value.push_back(word);
                        }
                    }
                    stored.push_back(temp);
                }
            }
            else if(start == 0)
            {
                if(word == ".data")         //data part starts
                    start = 1;
            }
        }
    }
    file.close();
    int pos = 0;
    for(int i = 0; i < stored.size(); i++)
    {

        seg temp;   temp.name = stored[i].name;    temp.position = pos + START;  
        datalabel.push_back(temp);
        if(stored[i].type == "byte")
        {
            for(int j = 0; j < stored[i].value.size(); j++)
            {
                string s = convert(stored[i].value[j],2);
                datamemory[pos++] = s;
            }
        }
        else if(stored[i].type == "word")
        {
            for(int j = 0; j < stored[i].value.size(); j++)
            {
                string s = convert(stored[i].value[j],8);
                string temp;    temp+=s[6]; temp+=s[7];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[4]; temp+=s[5];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[2]; temp+=s[3];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[0]; temp+=s[1];
                datamemory[pos++] = temp; temp.clear();
            }
        }
        else if(stored[i].type == "halfword")
        {
             for(int j = 0; j < stored[i].value.size(); j++)
            {
                string s = convert(stored[i].value[j],4);
                string temp;    
                temp+=s[2]; temp+=s[3];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[0]; temp+=s[1];
                datamemory[pos++] = temp; temp.clear();
            }
        }

    }
}


void formats()
{
	ifstream myFile;
	myFile.open("Format.txt");
	string line;
	while(getline(myFile,line))
	{
		Format.push_back(line);
	}
	sizeI = Format.size();
	myFile.close();
}


char h(ll ind)
{
	if(ind<=9)
		return ind+48;
	else
		return ind-10+65;
}

//Return a numerical value of whose digits are stored in vector
ll getnum(vector<int>temp,ll giv)
{
	ll size2 = temp.size();
	ll num=1;
	ll ans=0;
	for(ll i=size2-1;i>=0;i--)
	{
		ans += num*temp[i];
		num*=giv;
	}
	return ans;
}

//To get numerical value of hexadecimal Format
ll gethex(vector<int>temp)
{
	ll size2 = temp.size();
	ll num = 1;
	ll ans = 0; 
	for(ll i=size2-1;i>1;i--)
	{
		if(temp[i]<16)
		{
			ans += temp[i]*num;
		}
		else if(temp[i]<=42)
		{
			ans += (temp[i]-7)*num;
		}
		else 
		{
			temp[i]-=32;
			ans += (temp[i]-7)*num;
		}
		num*=16;
	} 
	return ans;
}

void hexa()
{
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	file<<"0x";
	string s;
	ll num =1;
	ll temppc = pccount;
	if(temppc==0)
	{
		s += '0';
	}
	while(temppc!=0)
	{
		char e = h(temppc%16);
		temppc/=16;
		s += e;
	}

	reverse(s.begin(),s.end());
	file<<s<<" ";
	file<<"0x";
	for(int i=0;i<32;i++)
	{
		vector<int>t;
		for(int j=0;j<4;j++)
		{
			t.push_back(binary[i++]);
		}
		ll num = getnum(t,2);
		char a = h(num);
		file<<a;
		i--;
	}
	file<<"\n";
	pccount+=4;
}


//To get all the Labels used in Code
int getlab(string label,int ind)
{
	int sizelabel = Label.size();
	for(int i=0;i<sizelabel;i++)
	{
		if(label.compare(Label[i].s)==0)
		{
			int dest = (Label[i].index-ind)*2;
			return dest;
		}
	}
}


void IFunction(int index,int index1)
{
	int size11 = code[index].size();
	int temp = 0;
	for(int i=0;i<size11;i++)
	{
		if(code[index][i]=='(')
		{
			temp=1;
			break;
		}
		if(code[index][i]=='#')
		{
			break;
		}
	}
    if(!temp)
	{
		int size = code[index].size();
		int size1 = Format[index1].size();
		ll rd,rs1,imme;
		int i=0;
		i++;
		while(code[index][i]!='x')
		{
			i++;
		}
		i++;
		vector<int>temp;
		while(code[index][i]!=' ' && code[index][i]!=',' )
		{
			temp.push_back(code[index][i]-48);
			i++;
		}
		rd = getnum(temp,10);
		temp.clear();
		while(code[index][i]!='x')
		{
			i++;
		}
		i++;
		while(code[index][i]!=' ' && code[index][i]!=',' )
		{
			temp.push_back(code[index][i]-48);
			i++;
		}
		rs1 = getnum(temp,10);
		temp.clear();
		while(code[index][i]==' ' || code[index][i]==',')
		{
			i++;
		}
		int flag1=0;
		if(code[index][i]=='-')	{flag1=1; i++;}
		int flag=0;
		while(i<size && code[index][i]!=' ' && code[index][i]!='#' && code[index][i]!=',' )
		{
			temp.push_back(code[index][i]-48);
			if(code[index][i]=='x')
				flag=1;
			i++;
		}
		if(flag==0)
		imme = getnum(temp,10);
		else
		imme = gethex(temp);
		if(flag1) imme = getinver(imme,12);
		temp.clear();
		i = 0;
		while(Format[index1][i]!=' ')
		{
			i++;
		}
		i++;
		int j;
		for(j=25;j<=31;j++)
			binary[j] = Format[index1][i++]-48;
		i++;
		j=24;
		for(int k=0;k<5;k++)
		{
			binary[j--] = rd%2;
			rd/=2;
		}
		for(j=17;j<=19;j++)
		{
			binary[j] = Format[index1][i++]-48;
		}
		i++;
		j=16;
		for(int k=0;k<5;k++)
		{
			binary[j--] = rs1%2;
			rs1/=2;
		}
		for(int k=0;k<12;k++)
		{
			binary[j--] = imme%2;
			imme/=2;
		}
		hexa();
		return;
	}
	else
	{
		int size = code[index].size();
		int size1 = Format[index1].size();
		ll rs1,rd,imme;
		int i=0;
		while(code[index][i]!='x')
		{
			i++;
		}
		i++;
		vector<int>temp;
		while(code[index][i]!=' ' && code[index][i]!=',' )
		{
			temp.push_back(code[index][i]-48);
			i++;
		}
		rd = getnum(temp,10);
		temp.clear();
		while(!(code[index][i]>=48 &&  code[index][i]<=57))
		{
			i++;
		}
		int flag1=0;
		if(code[index][i-1]=='-'){flag1=1;}
		int flag=0;
		while(code[index][i]!='(')
		{
			temp.push_back(code[index][i]-48);
			if(code[index][i]=='x')	flag=1;
			i++;
		}
		if(flag==0)
		imme = getnum(temp,10);
		else
		imme = gethex(temp);
		temp.clear();
		if(flag1) imme = getinver(imme,12);
		while(code[index][i]!='x')
		{
			i++;
		}
		i++;
		while(i<size && code[index][i]!=')')
		{
			temp.push_back(code[index][i]-48);
			i++;
		}
		rs1 = getnum(temp,10);
		temp.clear();
		i = 0;
		while(Format[index1][i]!=' ')
		{
			i++;
		}
		i++;
		int j;
		for(j=25;j<=31;j++)
			binary[j] = Format[index1][i++]-48;
		i++;
		j=24;
		for(int k=0;k<5;k++)
		{
			binary[j--] = rd%2;
			rd/=2;
		}
		for(j=17;j<=19;j++)
		{
			binary[j] = Format[index1][i++]-48;
		}
		i++;
		j=16;
		for(int k=0;k<5;k++)
		{
			binary[j--] = rs1%2;
			rs1/=2;
		}
		for(int k=0;k<12;k++)
		{
			binary[j--] = imme%2;
			imme/=2;
		}
		hexa();

	}
}

void SFunction(int index,int index1)
{
	int size = code[index].size();
	int size1 = Format[index1].size();
	ll rs1,rs2,imme;
	int i=0;
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	vector<int>temp;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rs2 = getnum(temp,10);
	temp.clear();
	while(!(code[index][i]>=48 &&  code[index][i]<=57))
	{
		i++;
	}
	int flag1=0;
	if(i-1>=0 && code[index][i-1]=='-')	flag1=1;
	int flag = 0;
	while(code[index][i]!='(')
	{
		temp.push_back(code[index][i]-48);
		if(code[index][i] == 'x')
			flag = 1;
		i++;
	}
	if(flag==0)
		imme = getnum(temp,10);
	else
		imme = gethex(temp);
	if(flag1) imme = getinver(imme,12);
	temp.clear();
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	while(i<size && code[index][i]!=')')
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rs1 = getnum(temp,10);
	temp.clear();
	i = 0;
	while(Format[index1][i]!=' ')
	{
		i++;
	}
	i++;
	int j;
	for(j=25;j<=31;j++)
		binary[j] = Format[index1][i++]-48;
	i++;
	j=24;
	for(int k=0;k<5;k++)
	{
		binary[j--] = imme%2;
		imme/=2;
	}
	for(j=17;j<=19;j++)
	{
		binary[j] = Format[index1][i++]-48;
	}
	i++;
	j=16;
	for(int k=0;k<5;k++)
	{
		binary[j--] = rs1%2;
		rs1/=2;
	}
	for(int k=0;k<5;k++)
	{
		binary[j--] = rs2%2;
		rs2/=2;
	}
	for(j=6;j>=0;j--)
	{
		binary[j] = imme%2;
		imme/=2;
	}
	hexa();
}

void RFunction(int index,int index1)
{
	int size = code[index].size();
	int size1 = Format[index1].size();
	ll rd,rs1,rs2;
	int i=0;
	i++;				//To get rid of x from xor instruction
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	vector<int>temp;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rd = getnum(temp,10);
	temp.clear();
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rs1 = getnum(temp,10);
	temp.clear();
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	while(i<size && code[index][i]!=' ' && code[index][i]!='#' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rs2 = getnum(temp,10);
	temp.clear();
	i = 0;
	while(Format[index1][i]!=' ')
	{
		i++;
	}
	i++;
	int j;
	for(j=25;j<=31;j++)
		binary[j] = Format[index1][i++]-48;
	i++;
	j=24;
	for(int k=0;k<5;k++)
	{
		binary[j--] = rd%2;
		rd/=2;
	}
	for(j=17;j<=19;j++)
	{
		binary[j] = Format[index1][i++]-48;
	}
	i++;
	j=16;
	for(int k=0;k<5;k++)
	{
		binary[j--] = rs1%2;
		rs1/=2;
	}
	for(int k=0;k<5;k++)
	{
		binary[j--] = rs2%2;
		rs2/=2;
	}
	for(j=0;j<=6;j++)
	{
		binary[j] = Format[index1][i++]-48;
	}
	hexa();
}


void UJFunction(int index,int index1)
{
	int size2 = code[index].size();
	int size1 = Format[index1].size();
	ll rd,imme;
	int i=0;
	string label;
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	vector<int>temp;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rd = getnum(temp,10);
	temp.clear();
	while(code[index][i]==' ' || code[index][i]==',')
	{
		i++;
	}

	while(i<size2 &&  code[index][i]!=' '&& code[index][i]!='#')
	{
		label += code[index][i];
		i++;
	}
	imme = getlab(label,index);
	if(imme<0)
	{
		imme = getinver(imme,20);
	}
	i=0;
	while(Format[index1][i]!=' ')
	{
		i++;
	}
	i++;
	int j;
	for(j=25;j<=31;j++)
		binary[j] = Format[index1][i++]-48;
	i++;
	j=24;
	for(int k=0;k<5;k++)
	{
		binary[j--] = rd%2;
		rd/=2;
	}
	j=10;
	for(int k=0;k<20;k++)
	{
		if(k<=9)
		{
			binary[j--] = imme%2;
			imme/=2;
		}
		else if (k==10)
		{
			binary[11] = imme%2;
			imme/=2;
			j=19;
		}
		else if (k!=19)
		{
			binary[j--] = imme%2;
			imme/=2;
		}
		else
		{
			binary[0] = imme%2;
			imme/=2;
		}
	}
	hexa();
}

void UFunction(int index,int index1)
{
	int size2 = code[index].size();
	int size1 = Format[index1].size();
	ll rd,imme;
	int i=0;
	string label;
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;

	vector<int>temp;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rd = getnum(temp,10);
	temp.clear();

	while(code[index][i]==' ' || code[index][i]==',' )
	{
		i++;
	}

	int flag=0;
	int flag1 = 0;

	if(code[index][i]=='-') {
		flag1 = 1;
		i++;
	}

	while(i<size2 && code[index][i]!=' '&& code[index][i]!='#')
	{
		temp.push_back(code[index][i]-48);
		if(code[index][i]=='x')
			flag=1;
		i++;
	}
	if(flag==0)
		imme = getnum(temp,10);
	else
		imme = gethex(temp);
	if(flag1)	imme = getinver(imme,20);
	i=0;
	while(Format[index1][i]!=' ')
	{
		i++;
	}
	i++;
	int j;
	for(j=25;j<=31;j++)
		binary[j] = Format[index1][i++]-48;
	i++;
	j=24;
	for(int k=0;k<5;k++)
	{
		binary[j--] = rd%2;
		rd/=2;
	}
	j=19;
	for(int k=0;k<20;k++)
	{
		binary[j--] = imme%2;
		imme/=2;
	}
	hexa();
}

void SBFunction(int index,int index1)
{
	int size2 = code[index].size();
	int size1 = Format[index1].size();
	ll rs1,rs2,imme;
	int i=0;
	string label;
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	vector<int>temp;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rs1 = getnum(temp,10);
	temp.clear();
	while(code[index][i]!='x')
	{
		i++;
	}
	i++;
	while(code[index][i]!=' ' && code[index][i]!=',' )
	{
		temp.push_back(code[index][i]-48);
		i++;
	}
	rs2 = getnum(temp,10);
	temp.clear();
	while(code[index][i]==' ' || code[index][i]==',' )
	{
		i++;
	}
	while(i<size2 && code[index][i]!=' ' && code[index][i]!='#')
	{
		label+= code[index][i];
		i++;
	}
	imme = getlab(label,index);

	if(imme<0)
	{
		imme = getinver(imme,12);
	}
	i=0;
	while(Format[index1][i]!=' ')
	{
		i++;
	}
	i++;
	int j;
	for(j=25;j<=31;j++)
		binary[j] = Format[index1][i++]-48;
	i++;
	for(j=17;j<=19;j++)
		binary[j] = Format[index1][i++]-48;
	j=16;
	for(int k=0;k<5;k++)
	{
		binary[j--] = rs1%2;
		rs1/=2;
	}
	for(int k=0;k<5;k++)
	{
		binary[j--] = rs2%2;
		rs2/=2;
	}
	j=23;
	for(int k=0;k<4;k++)
	{
		binary[j--] = imme%2;
		imme/=2;
	}
	j=6;
	for(int k=0;k<6;k++)
	{
		binary[j--] = imme%2;
		imme/=2;
	}
	binary[24]=imme%2;
	imme/=2;
	binary[0]=imme%2;
	imme/=2;
	hexa();
}

void typenumber(string ins,int index,int index1)
{
	if(ins=="I")
		IFunction(index,index1);
	if(ins=="R")
		RFunction(index,index1);
	if(ins=="S")
		SFunction(index,index1);
	if(ins=="UJ")
		UJFunction(index,index1);
	if(ins=="U")
		UFunction(index,index1);
	if(ins=="SB")
		SBFunction(index,index1);
}

//To extract instruction type and process them independently
void process()
{
	size = code.size();
	for(int i=0;i<size;i++)
	{
		string ins;
		int j=0;
		while(j<code[i].size() && code[i][j]!=' ')
		{
			ins += code[i][j];
			j++;
		}

		int sins = ins.size();
		if(ins[sins-1]==':' && code[i].size() > sins)
		{
			ins.clear();
			while(j<code[i].size() && code[i][j]==' ')
				j++;
			while(j<code[i].size() && code[i][j]!=' ')
				ins += code[i][j++];
		}
		for(int k=0;k<sizeI;k++)
		{
			string type;
			int k1=0;
			while(Format[k][k1]!=' ')
			{
				type += (Format[k][k1]);
				k1++;
			}  
			if(ins.compare(type)==0)
			{
				int size1 = Format[k].size();
				k1 = size1-1;
				string type1;
				while(Format[k][k1]!=' ')
				{
					type1 += (Format[k][k1]);
					k1--;
				}
				reverse(type1.begin(),type1.end());
				typenumber(type1,i,k);
				break;
			}
		}

	}
}


//To convert Stack Pointer(sp) to x2
void preprocess()
{
	for(int i=0;i<code.size();i++)
	{
		int inssize = code[i].size();
		for(int j=1;j<inssize;j++)
		{
			if(code[i][j-1]==' ' && code[i][j]=='s' && j+1<inssize && code[i][j+1]=='p' && j+2<inssize && code[i][j+2]==' ')
			{
				code[i][j] = 'x';
				code[i][j+1] = '2';
			}
			if(code[i][j-1]=='(' && code[i][j]=='s' && j+1<inssize && code[i][j+1]=='p' && j+2<inssize && code[i][j+2]==')')
			{
				code[i][j] = 'x';
				code[i][j+1] = '2';
			}
			if(code[i][j-1]==' ' && code[i][j]=='s' && j+1<inssize && code[i][j+1]=='p' && j+2<inssize && code[i][j+2]==',')
			{
				code[i][j] = 'x';
				code[i][j+1] = '2';
			}
			if(code[i][j-1]==',' && code[i][j]=='s' && j+1<inssize && code[i][j+1]=='p' && j+2<inssize && code[i][j+2]==',')
			{
				code[i][j] = 'x';
				code[i][j+1] = '2';
			}
		}
	}
}

//To process Load Address(la) psudo command
void processla(int index)
{
	int i = 0;
	while(codeinit[index][i]!='x')
		i++;
	i++;
	string s;
	while(codeinit[index][i]!=' ')
		s+=codeinit[index][i++];
	string ins;
	ins = "auipc x";
	ins += s;
	ins += " 65536";
	code.push_back(ins);
	int currentpc = code.size()*4-4;
	currentpc += START;
	while(codeinit[index][i]==' ')
		i++;
	string labeltype;
	while(i<codeinit[index].size() && codeinit[index][i]!=' ')
		labeltype+=codeinit[index][i++];
	ll labeladdress= 0;
	for(int j = 0; j<datalabel.size();j++)
	{
		if(labeltype.compare(datalabel[j].name)==0)
		{
			labeladdress = datalabel[j].position;
			break;
		}
	}
	labeladdress = labeladdress - currentpc;
	string labeladd;
	ll  temp1 = abs(labeladdress);
	while(temp1!=0)
	{
		labeladd += (temp1%10)+48;
		temp1/=10;
	}
	if(labeladdress<0)
		labeladd+='-';
	else if(labeladdress==0)
	{
		labeladd+='0';
	}
	reverse(labeladd.begin(),labeladd.end());
	ins.clear();

	ins = "addi x" +  s + " x" + s + " " + labeladd;
	code.push_back(ins);
}

//To process Load Word (lw) psudo command
void processlw(string type,int index,ll pos)
{
	ll currentpc = code.size()*4 + START;
	int i = 0;
	while(codeinit[index][i]!='x')
		i++;
	i++;
	string s;
	while(codeinit[index][i]!=' ')
		s+=codeinit[index][i++];
	currentpc = pos - currentpc;
	string ins;
	ins = "auipc x" + s +" 65536";
	code.push_back(ins);	ins.clear();
	string labeladd;
	ll  temp1 = abs(currentpc);
	while(temp1!=0)
	{
		labeladd += (temp1%10)+48;
		temp1/=10;
	}
	if(currentpc<0)
		labeladd+='-';
	else if(currentpc==0)
	{
		labeladd+='0';
	}
	reverse(labeladd.begin(),labeladd.end());
	ins = type+ " x" + s + " " + labeladd + "(x" + s + ")";
	code.push_back(ins);
}

//To expand all psudo instruction if present
void shift()
{
	int siz = codeinit.size();
	for(int i=0;i<siz;i++)
	{
		for(int j=0;j<codeinit[i].size();j++)
		{
			if(codeinit[i][j]==9)
			{
				codeinit[i][j]=32;
			}
		}
		string ins;
		int j=0;
		int start = -1;
		while(j<codeinit[i].size()  && codeinit[i][j]==' ')
		{
			j++;
		}
		start = j;
		while(j<codeinit[i].size() && codeinit[i][j]!=' ')
		{
			ins += codeinit[i][j];
			j++;
			if(j<codeinit[i].size() && codeinit[i][j]==':')
			{
				ins+=codeinit[i][j];
				j++;
				break;
			}
		}

		while(j<codeinit[i].size()  && codeinit[i][j]==' ')
			j++;
		int sins = ins.size();
		if(ins[sins-1]==':' && codeinit[i].size() > sins)
		{
			ins.clear();
			while(j<codeinit[i].size() && codeinit[i][j]==' ')
			j++;
			start = j;
			while(codeinit[i][j]!=' ')
				ins += codeinit[i][j++];
		}
		if(ins == "la")
		{
			processla(i);
			continue;
		}
		else if(ins == "lw" || ins== "lb" || ins == "lhw")
		{
			string lab;
			int j = codeinit[i].size() - 1;
			while(codeinit[i][j]==' ')
			{
				j--;
			}
			while(codeinit[i][j]!=' ')
			{
				lab+=codeinit[i][j];
				j--;
			}
			reverse(lab.begin(),lab.end());
			int flag = 1;
			for(int j =0 ; j<datalabel.size() ; j++)
			{
				if(lab.compare(datalabel[j].name)==0)
				{
					flag = 0;
					processlw(ins,i,datalabel[j].position);
					break;
				}
			}
			if(!flag)	continue;
		}
		for(int k=0;k<sizeI;k++)
		{
			string type;
			int k1=0;
			while(Format[k][k1]!=' ')
			{
				type += (Format[k][k1]);
				k1++;
			}  
			if(ins.compare(type)==0)
			{
					string add;
					for(int k = start;k<codeinit[i].size();k++)
					{
						add += codeinit[i][k];
					}
					code.push_back(add);
				break;
			}
		}

	}
}

//To extract all the labels from code
void setlabel()
{
	int siz = codeinit.size();
	int count = -1;
	for(int i=0;i<siz;i++)
	{
		string ins;
		int j=0;
		int start = -1;
		while(j<codeinit[i].size()  && codeinit[i][j]==' ')
		{ 
			j++;
		}
		start = j;
		while(j<codeinit[i].size() && codeinit[i][j]!=' ')
		{ 
			ins += codeinit[i][j];
			j++;
			if(j<codeinit[i].size() && codeinit[i][j]==':')
			{
				ins+=codeinit[i][j];
				j++;
				break;
			}
		}
		int sins = ins.size();
		if(ins[sins-1]==':')
		{
			string ins1;
			for(int k = 0 ; k<sins-1 ; k++)
				ins1+=ins[k];
			lab temp;
			temp.s = ins1;
			temp.index = count+1;
			Label.push_back(temp);
		}
		if(ins[sins-1]==':' && sins<codeinit[i].size())
		{
			while(j<codeinit[i].size() && codeinit[i][j]==' ')
			{
				j++;
			}
			ins.clear();
			while(j<codeinit[i].size() && codeinit[i][j]!=' ')
			{
				ins += codeinit[i][j];
				j++;
			}
		}
		if(ins == "la")
		{
			count+=2;
			continue;
		}
		else if(ins == "lw" || ins == "lb" || ins =="lhw")
		{
			string lab;
			int j = codeinit[i].size() - 1;
			while(codeinit[i][j]==' ')
			{
				j--;
			}
			while(codeinit[i][j]!=' ')
			{
				lab+=codeinit[i][j];
				j--;
			}
			reverse(lab.begin(),lab.end());
			int flag = 1;
			for(int j =0 ; j<datalabel.size() ; j++)
			{
				if(lab.compare(datalabel[j].name)==0)
				{
					flag = 0;
					count += 2;
					break;
				}
			}
			if(!flag)	continue;
		}

		for(int k=0;k<sizeI;k++)
		{
			string type;
			int k1=0;
			while(Format[k][k1]!=' ')
			{
				type += (Format[k][k1]);
				k1++;
			}  
			if(ins.compare(type)==0)
			{
				count++;
				break;
			}
		}

	}
}

//Driver Code
int main()
{
	for(int i=0;i<4000;i++)
		datamemory[i] = "00";
	read_data(); 
	
	ofstream files;
	files.open("MCode.mc");
	files.close();
	formats();
	ifstream myFile;
	myFile.open("test.asm");
	string line;
	int flag = 0;
	int start = 0;
	while(getline(myFile,line))
	{
		if(line==".data")
			flag = 1;
		if(line==".text")
		{
			flag = 0;
			continue;
		}
		if(flag!=1)
			codeinit.push_back(line);
	}
	shift();
	setlabel();
	preprocess();
	process();
	myFile.close();
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string s = "-------------------------------------------------------";
	file<<s<<endl;

	//Print the Data Memory Part in Increasing Address Order
	for(int i=0;i<400;i++)
	{
		file<<datamemory[i]<<" ";
		if((i+1)%4==0)
		{
			file<<endl;
		}
	}
	file<<s<<endl;
	file.close();


}