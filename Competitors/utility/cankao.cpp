#define CANKAO
#include "cankao.hpp"



std::set<VertexType> vertex;
VertexType maxVertex;

std::vector<VertexType> asrc, adst;
int cnt;


void LoadDataSparseDense(std::string path)
{
    using namespace std;
    int intbig = 2147483647;

    ifstream infile;
    infile.open(path);
    if (!infile.is_open())
    {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(1);
    }
    int i = 0;
    maxVertex = 0;
    std::cout << "Data set loading begin\n";
    while (1)
    {

        string src;
        string dst;
        if (!getline(infile, src, ','))
            break;
        getline(infile, dst);

        asrc.push_back(stoi(src));
        adst.push_back(stoi(dst));
        maxVertex = std::max(asrc[i], maxVertex);
        maxVertex = std::max(adst[i], maxVertex);
        vertex.insert(asrc[i]);
        vertex.insert(adst[i]);
        ++i;
    }
    cnt = i;

    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;
    std::cout << "load data cnt: " << cnt << endl;
    std::cout << "maxVertex: " << maxVertex << endl;
    infile.close();
}

void LoadDataCaida(std::string path)
{
    using namespace std;
    ifstream input("/share/datasets/CAIDA2018/dataset/130000.dat", ios::in | ios::binary);
    char buf[200] = {0};
    cnt = 0;
    int MAXNUM = 5 * 1e7;
    int length = MAXNUM;
    for (cnt = 0; cnt < length; ++cnt)
    {
        if (!input.read(buf, 21))
        {
            break;
        }
        asrc.push_back(*(int *)buf);
        adst.push_back(*(int *)(buf + 4));
    }

    unordered_map<key_type, key_type> mp;
    int node_num = 0;
    for (int it = 0; it < cnt; ++it)
    {
        if (!mp.count(asrc[it]))
        {
            ++node_num;
            mp.insert({asrc[it], node_num});
        }
        if (!mp.count(adst[it]))
        {
            ++node_num;
            mp.insert({adst[it], node_num});
        }
    }
    maxVertex = 0;
    cout << "caida node num:  " << node_num << endl;
    for (int it = 0; it < cnt; ++it)
    {
        asrc[it] = mp[asrc[it]];
        adst[it] = mp[adst[it]];
        maxVertex = std::max(asrc[it], maxVertex);
        maxVertex = std::max(adst[it], maxVertex);
        vertex.insert(asrc[it]);
        vertex.insert(adst[it]);
    }

    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;
    std::cout << "load data cnt: " << cnt << endl;
    std::cout << "maxVertex: " << maxVertex << endl;
}

// notredame
void LoadDataNotredame(std::string path)
{
    using namespace std;
    std::ifstream infile(path);
    if (!infile.is_open())
    {
        std::cout << "Cannot open the file：" << path << std::endl;
        exit(1);
    }
    std::cout << "Loading Dataset:" << path << std::endl;
    maxVertex = 0;
    int intbig = 2147483647;
    int i = 0;

    char useless[50];
    infile.getline(useless, intbig);

    infile.getline(useless, intbig); 
    infile.getline(useless, intbig); 
    infile.getline(useless, intbig); 

    while (1)
    {
        char s[50];
        if (!infile.getline(s, intbig))
            break;
#ifdef DEBUG
        if (i > 100000)
        {
            break;
        }
#endif
        std::string src = "";
        std::string dst = "";
        int d = 0;

        for (int k = 0; k < 50;)
        {
            if (s[k] >= '0' && s[k] <= '9')
            {
                int r = k;
                while (s[r] >= '0' && s[r] <= '9')
                {
                    std::string cc;
                    cc = s[r];
                    if (d == 0)
                    {
                        src.append(cc);
                        ++r;
                    }
                    else if (d == 1)
                    {
                        dst.append(cc);
                        ++r;
                    }
                }
                k = r;
                if (d == 0)
                    ++d;
                else if (d == 1)
                    k = 50;
            }
            else
                ++k;
        }
        asrc.push_back(stoi(src));
        adst.push_back(stoi(dst));
        maxVertex = std::max(asrc[i], maxVertex);
        maxVertex = std::max(adst[i], maxVertex);
        vertex.insert(asrc[i]);
        vertex.insert(adst[i]);
        ++i;
    }
    cnt = i;
    std::cout << "load data cnt: " << cnt << endl;
    infile.close();
    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;
    std::cout << "asrc[5]: " << asrc[5] << std::endl;
    std::cout << "adst[5]: " << adst[5] << std::endl; 
}



// wiki
void LoadDataWiki(std::string path)
{
    using namespace std;
    std::ifstream infile(path);
    if (!infile.is_open())
    {
        std::cout << "Cannot open the file：" << path << std::endl;
        exit(1);
    }
    std::cout << "Loading Dataset:" << path << std::endl;
    maxVertex = 0;
    int intbig = 2147483647;
    int i = 0;

    char useless[50];
    infile.getline(useless, intbig);

    while (1)
    {
        char s[50];
        if (!infile.getline(s, intbig))
            break;
#ifdef DEBUG
        if (i > 100000)
        {
            break;
        }
#endif
        std::string src = "";
        std::string dst = "";
        int d = 0;

        for (int k = 0; k < 50;)
        {
            if (s[k] >= '0' && s[k] <= '9')
            {
                int r = k;
                while (s[r] >= '0' && s[r] <= '9')
                {
                    std::string cc;
                    cc = s[r];
                    if (d == 0)
                    {
                        src.append(cc);
                        ++r;
                    }
                    else if (d == 1)
                    {
                        dst.append(cc);
                        ++r;
                    }
                }
                k = r;
                if (d == 0)
                    ++d;
                else if (d == 1)
                    k = 50;
            }
            else
                ++k;
        }
        asrc.push_back(stoi(src));
        adst.push_back(stoi(dst));
        maxVertex = std::max(asrc[i], maxVertex);
        maxVertex = std::max(adst[i], maxVertex);
        vertex.insert(asrc[i]);
        vertex.insert(adst[i]);
        ++i;
    }
    cnt = i;
    std::cout << "load data cnt: " << cnt << endl;
    infile.close();
    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;
    std::cout << "asrc[5]: " << asrc[5] << std::endl;
    std::cout << "adst[5]: " << adst[5] << std::endl; 
}

void LoadDataStackOverflow(std::string path) {
    using namespace std;
    std::ifstream infile(path);
    if (!infile.is_open()) {
        std::cout << "Cannot open the file：" << path << std::endl;
        exit(1);
    } 
    std::cout << "Loading Dataset:" << path << std::endl;
    maxVertex = 0;
    int intbig = 2147483647;
    int i = 0;
    
    char useless[50];
    infile.getline(useless, intbig);
    
    
    while (1) {
        char s[50];
        if (!infile.getline(s, intbig)) break;
        #ifdef DEBUG
        if (i > 100000) {
            break;
        }
        #endif
        std::string src = "";
        std::string dst = "";
        int d = 0;

        for (int k = 0; k < 50;) {
            if (s[k] >= '0' && s[k] <= '9') {
                int r = k;
                while (s[r] >= '0' && s[r] <= '9') {
                    std::string cc; cc = s[r];
                    if (d == 0) { src.append(cc); ++r; }
                    else if (d == 1) { dst.append(cc); ++r; }
                }
                k = r;
                if (d == 0)++d;
                else if (d == 1)k = 50;
            }
            else ++k;
        }
       asrc.push_back(stoi(src));
       adst.push_back(stoi(dst));
        maxVertex = std::max(asrc[i], maxVertex);
        maxVertex = std::max(adst[i], maxVertex);
        vertex.insert(asrc[i]);
        vertex.insert(adst[i]);
        ++i;
    }
    cnt = i;
    std::cout <<"load data cnt: "<<cnt<<endl;
    infile.close();
    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;
    std::cout<<"asrc[5]: "<<asrc[5]<<std::endl;std::cout<<"adst[5]: "<<adst[5]<<std::endl;//////
}



void sina_split(const std::string& s, char delimiter, std::vector<std::string>& v)
{
	std::string::size_type i = 0;
	std::string::size_type j = s.find(delimiter);
	while (j != std::string::npos)
	{
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(delimiter, i);
	}
	if (j == std::string::npos)
		v.push_back(s.substr(i, s.length()));
}

void LoadDataSina(std::string path) {
    using namespace std;
    cout << "sina begin!" << endl;

    int intbig = 2147483647;

    ifstream infile;
    infile.open(path);
    int i = 0;
    int strange = 0;
    while (1) {
        string src;
        if (!getline(infile, src)) break;
        if (src[0] < '0' || src[0]>'9')continue;
        vector<string> aaa;
        sina_split(src, ' ', aaa);
        if (aaa.size() == 2) {
            key_type u = stoi(aaa[0]);
            key_type v = stoi(aaa[1]);
            asrc.push_back(u);
            adst.push_back(v);
        }
        else if (aaa.size() >= 3) {
            ++strange;
            key_type u = stoi(aaa[1]);
            key_type v = stoi(aaa[2]);
            asrc.push_back(u);
            adst.push_back(v);
        }


        ++i;

    }
    cnt = i;

    cout << "i:  " << i << endl;
    cout << "strange:  " << strange << endl;

    unordered_map<key_type, key_type> mp;
    int node_num = 0;
    for (int it = 0; it < cnt; ++it) {
        if (!mp.count(asrc[it])) {
            ++node_num;
            mp.insert({ asrc[it], node_num });
        }
        if (!mp.count(adst[it])) {
            ++node_num;
            mp.insert({ adst[it], node_num });
        }
    }
    maxVertex = 0;
    cout << "stack node num:  " << node_num << endl;
    for (int it = 0; it < cnt; ++it) {
        asrc[it] = mp[asrc[it]];
        adst[it] = mp[adst[it]];
        maxVertex = std::max(asrc[it], maxVertex);
        maxVertex = std::max(adst[it], maxVertex);
        vertex.insert(asrc[it]);
        vertex.insert(adst[it]);
    }


    std::cout << "load data cnt: " << cnt << endl;
    infile.close();
    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;

    infile.close();
}