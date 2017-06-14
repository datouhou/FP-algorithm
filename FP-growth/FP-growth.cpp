#include <cstdio>  
#include<string>  
#include <cstring>
#include<map>  
#include<vector>        
#include<iostream>  
#include <fstream>
#include<list> 
#include<cmath> 
using namespace std;
 
 
  
  
class DataNode{//定义数据节点，用于记录每件商品的名称和次数   
   public:
    int name;//用整型数据name映射商品名称的字符串 
    int num; //用于记录商品的数量 
    DataNode(int x,int y){//自定义数据节点的生成函数 
        name=x;  
        num=y;  
    }  
};  
class TreeNode{//定义FP-tree的树节点类   
   public:
    int parent;//父节点的节点编号 
    vector<int>child;//子节点的节点编号 
    int name;//节点商品的名称编号 
	int num;//节点商品的数量 
}; 
TreeNode Copy(TreeNode tmp){//定义树节点的拷贝函数，方便之后进行递归的状态保存和恢复   
    TreeNode orginal;//原始节点  
    orginal.parent=tmp.parent;//拷贝父节点 
    orginal.name=tmp.name;//拷贝节点商品的名称编号  
	orginal.num=tmp.num;//拷贝节点商品的数量  
    return orginal;  
} 
typedef list<DataNode>  li_d;  
int data_size=9835;//数据的行数 
int minsupport=50;//最小支持数 
int tree_sz;//FP树的节点数量  
int total;//数据中商品的种类数 
int count[10000];//每类商品的数量  
int weight[10000];//商品的权重数组，用于建树之后对每项进行排序的依据
int binary[200];
int path_num; 
int cond_node_num;
ofstream outfile;
int dictionary[100000][200];//将树结构字典化，用于记录每个树节点的子节点，减少遍历，空间换时间 
map<string,int> index;//用于商品名称和商品编号的映射 
li_d DataBase[10000];//数据库，用于记录每轮建树的数据 
TreeNode *cond_tree=new TreeNode[50000];//记录FPtree结构 

string getname(int item){//输入：商品编号，输出：商品名称的字符串  
    string s;//定义字符串，保存商品名称
	/*循环检索index，进行商品的编号的匹配，还原商品名*/  
    for(map<string,int>::iterator it=index.begin();it!=index.end();it++)  
    {  
        if(it->second==item)//如果检索到了匹配的编号，就将对应的商品民字符串匹配输出 
        {  
            s=it->first;  
            return s;  
        }  
    }
    
	s="This is something wrong with index,we can't find it.";//如果未找到，说明index出现错误，提交错误信息  
    return s;  
}  
void Input(){//输入函数，用于导入实验数据,将初始化的数据保存在DataBase数组中  
	ifstream f("./project4 - Groceries - 2.csv");//打开文件输入流，导入数据文件 
	
	/*当数据文件打开失败的时候，直接打印错误信息，退出程序*/ 
	if(f.fail()){
		cout << "Connot open file"<<endl;
		return;
	}
	
	string line;//定义字符串，保存读入的原始数据 
	int num=1;//定义编码整型，记录商品的编码 
	string s="";//定义字符串，记录处理数据后，得到的商品名称 
	std::getline(f,line);//读取第一行数据，将第一行的条目排除 
	int i = 0;//记录当前读取数据的行数，方面DataBase进行索引
	
	/*每次读取一行数据，并进行处理，保存在DataBase中*/ 
	while(std::getline(f,line)){
		i++;//每次读取行数加一 
		bool flag = true;//定义布尔型，用于判断是否读到'{','{'是商品名称的开始 
		for(int l = 0; l < line.length();l++){
			if(flag){//flag为true说明还未读到商品名称,为false说明读到商品名称，开始处理商品名 
		       if(line[l]!='{')	continue;//没有读到'{'，保持flag状态，继续向下查找 
   			   else {//读到'{'，改变flag状态 
				  flag = false;
				  continue;
	  		    }
			}
			/*没有读到','或者'}'说明当前商品名还未结束，继续s的延长*/ 
			if(line[l]!=','&& line[l]!='}'){
				s+=line[l];
			}
			
			/*否则，当前商品名结束，开始保存到DataBase中*/ 
			else{
				map<string,int>::iterator index_it;//定义迭代器，用于保存商品名称查找的位置  
            	index_it=index.find(s);//在index中查找当前的商品名称
				  
            	if(index_it==index.end()){//迭代器出现在index结尾，说明未找到当前名称，需要新建映射 
	                index[s]=num;//建立新的映射关系  
	                num++;//商品编号加1  
            	}  
             	DataBase[i].push_back(DataNode(index[s],1));//将当前的编号和数量1保存在DataBase当前记录中    
				s="";//清空字符串s，用于下一个商品名称的记录 
			}
			/*如果读取到'}'，当前交易记录读取结束，离开循环，进行下一条的读取*/ 
			if(line[l]=='}'){
				break;
			}
		}
	}     
	total = num;
	//printf("%d\n",total);//打印商品种类的总数量，用于调试，防止数组溢出 
}  


bool cmp(DataNode a,DataNode b){//定义比较函数，比较数据节点的商品数量，进行headtable的排序
    return a.num>=b.num;  
}  

 
li_d  CreateHeadTable(li_d *database,int datasize){//函数用于创建数据头表   
    memset(count,0,sizeof(count));//清空计数数组，准备记录每类商品的数量  
    li_d headtable;//创建数据节点列表，用于记录headtable
	 
	/*遍历每一条交易数据中的每一个商品，统计每类商品的总数量*/ 
    for(int i=0;i<datasize;i++)
    {  
           
         for(li_d ::iterator iter=database[i].begin();iter!=database[i].end();iter++)//遍历每一条交易记录的每个商品  
         {    
             count[iter->name]+=iter->num;//将当前商品的数目加1 
         }  
    } 
	
	/*遍历每一类商品，将其中满足最小支持数的商品筛选出来*/ 
    for(int i=0;i<total;i++)  
    {  
        if(count[i]<minsupport) continue;  
        headtable.push_back(DataNode(i,count[i]));//如果商品数大于最小支持树，将该数据加入到headtable中 
    }  
    headtable.sort(cmp);//按照商品数量对headtable中的每一项进行排序，从大到小 
	return headtable;
}


bool cmp2(DataNode x,DataNode y){//定义比较函数，用于对每条交易记录的商品进行内部排序  
    if(weight[x.name]<=weight[y.name])
        return true;  
    return false;  
}  
void sort_weight(li_d *database,li_d headtable,int datasize){//对weight数据进行构建，并利用该数组对每条交易记录的商品进行内部排序 
	
    memset(weight,0,sizeof(weight));//清空权重数组，用于内部排序  
    int order=0;//初始化权重编号
	  
	/*遍历项头表，对权重数据进行构建*/  
    for(li_d ::iterator iter=headtable.begin();iter!=headtable.end();iter++){    
        weight[iter->name]=order;//将当前权重付给对应商品 
        order++; //权重编号顺次加1 
    } 
	
 	/*遍历数据库，对数据库中的每一条交易记录进行检查，剔除小于最小支持数的，加快计算 */
    for(int i=0;i<datasize;i++){//遍历数据库  
         for(li_d ::iterator iter=database[i].begin();iter!=database[i].end();){//遍历每一条交易记录   
             if(count[iter->name]<minsupport){//如果当前商品的数量小于最小支持度，就将其从交易记录中删除  
                 database[i].erase(iter++);  
             }  
             else  
            {  
                iter++;  
            }  
         }  
         database[i].sort(cmp2);//根据cmp2,对当前的交易记录进行排序 
    }  
}
 
TreeNode* BuildTree(li_d a [],int num,TreeNode *FP_tree){//利用当前的数据列表进行FP树的构建  
    //TreeNode *Val=new TreeNode[100];//用数组存储数节点，可以通过每个节点的parent和child还原整个树，Val[0]存根节点  
    memset(dictionary,0,sizeof(dictionary));//清空dictionary数组，用于记录当前的节点父子关系  
    int node_num=1;//记录当前FP树的节点数  
    
	/*遍历当前数据，构建树结构*/ 
	for(int i=0;i<num;i++){//遍历当前数据的每一行  
        int root=0;//每条记录的构建都以null节点开始 
        for(li_d ::iterator it=a[i].begin();it!=a[i].end();it++){//遍历每一条交易记录  
            DataNode t=*it;  
            int s=t.name;
			
			/*如果遍历到的商品不是当前节点的子节点，新增子节点*/  
            if(dictionary[root][s]==0){  
                memset(dictionary[node_num],0,sizeof(dictionary[node_num]));//清空当前结点编码的dictionary列表，用于记录它的子节点  
                TreeNode tmp=TreeNode();//新建节点  
                tmp.num=t.num;//赋值操作  
                tmp.name=s;  
                tmp.parent=root;//父节点定义为当前节点  
                FP_tree[node_num]=tmp;//新增节点赋值给当前节点编号的树节点  
                FP_tree[root].child.push_back(node_num);//在当前的树节点的子节点列表中中添加新增节点 
                dictionary[root][s]=node_num;//更新当前节点的dictionary数据，方便之后的查找操作 
                node_num++;//节点数据加1 
            }
			
			/*遍历到的商品是当前节点的子节点，则将FP树中对应节点的数量加1*/  
            else{  
                for(int k=0;k<FP_tree[root].child.size();k++){  
                    if(FP_tree[FP_tree[root].child[k]].name==s){//查找到对应商品的子节点  
                        FP_tree[FP_tree[root].child[k]].num+=t.num;//将商品数量增加 
                         break;  
                    }  
                }  
            }  
            root=dictionary[root][s];//将当前节点沿路径向下移动 
        }  
    }  
    tree_sz=node_num;//更新树的大小 
    return FP_tree; 
}  

void one_path_out(li_d database[],int leaf_node,int cond_code_num,li_d  post_index){//用于当前树只剩下一条路径时，直接输出路径上的全部排列组合，减少递归 
	//cout <<"cond_code_num"<<cond_code_num<<endl;
	string str="";//定义字符串，用于记录当前的前缀 
	if(!post_index.empty()){//前缀不为空    
            for(li_d ::iterator it=post_index.begin();it!=post_index.end();it++){//循环得到前缀集    
                str+=getname(it->name)+" "; 
            }      
    }  
	memset(binary,0,sizeof(binary));//清空binary数组，用于模拟二进制进位 
	int bit=0;//定义整型用于控制循环次数
	
	/*模拟二进制进位过程，进行路径节点组合的枚举*/ 
	while(bit<pow(2,cond_code_num)){//循环次数应等于2的cond_code_num次方 
		bit++;
		binary[0]++;//二进制末位加1 
		string s="";//定义字符串用于当前记录当前的频繁项 
		int j = 0;//定义整型，用于索引binary数组 
		int num = cond_tree[leaf_node].num;//获取当前频数 
		for(li_d ::iterator iter=database[0].begin();iter!=database[0].end();iter++){
			if(binary[j]==1)	s+=getname(iter->name)+" ";//字符串加上当前二进制位为1的对应的商品 
			j++;//检查二进制下一位 
		}
		s+=str;//加上前缀集 
	    outfile<<s<< num << endl;//输出 
		int i = 0;//定义整型，用于进位移动 
		while(binary[i]>1){//检查当前位是否等于2 
			binary[i]=0;//等于2就将当前位变为0 
			i++;//前进一位 
			binary[i]+=1;//当前位加1 
		}
	}
	
}
void FP_Growth(li_d database[],li_d post_index,int data_size){//通过递归，实现FP-growth算法，在FP树中寻找频繁项 
    li_d head_table=CreateHeadTable(database,data_size);//利用函数CreateHeadTable，创建项表头 
    sort_weight(database,head_table,data_size);//对每条交易记录内部进行排序 
    BuildTree(database,data_size,cond_tree);//构建FP树 
    TreeNode *tree_buffer=new TreeNode[50000];//创建FPtree的buffer数组，用于在递归中记录FP树并在递归结束是还原 
	int tmpsz=tree_sz;//记录当前FP树的规模，用于FP树恢复时候循环的边界 
    
	/*拷贝FP树*/ 
	for(int i=0;i<tree_sz;i++){  
        tree_buffer[i]=Copy(cond_tree[i]);  
    }
	
	/*如果当前构造出的树只有一个根节点，说明树结构为空，递归结束*/  
    if(tree_sz==1){  
        return;  
    }
	/*遍历head_table，对当前的频繁项集进行输出,因为查找关联关系，当前缀中没有元素的时候，频繁项不必输出*/ 
    if(!post_index.empty()){  
        for(li_d ::iterator iter=head_table.begin();iter!=head_table.end();iter++){//遍历head_table  
            DataNode t=*iter;  
            string str=getname(t.name);
            int num=t.num;
            for(li_d ::iterator it=post_index.begin();it!=post_index.end();it++){//循环输出前缀集    
                str+=","+getname(it->name); 
            }
            outfile<<str<<" "<<num<<endl;  
        }  
    }  
    
    /*按照数量从小到大的顺寻，对head_table进行遍历，从树的叶节点开始，依次作为前缀，向上查找，构建条件FPtree*/ 
    for(li_d ::reverse_iterator iter=head_table.rbegin();iter!=head_table.rend();iter++){  
        DataNode t=*iter;  
        li_d new_post_index;//定义新的前缀列表 
        new_post_index.push_back(DataNode(t.name,t.num));//将当前遍历节点加入到前缀列表中 
        if(!post_index.empty()){//当前前缀列表非空，要与新的列表进行合并操作  
            li_d tmp=post_index;  
            new_post_index.merge(post_index,cmp);  
            post_index=tmp;  
        }  
        li_d newdatabase[10000];//定义数据节点列表，用于下一轮递归的数据保存 
        int new_data_size=0;
        path_num=0;
        cond_node_num=0;
        int leaf_node;
		/*循环遍历当前的树节点，将其中的所有当前遍历商品找出，并向上查找所有关联路径，构建新的数据*/  
        for(int i=0;i<tree_sz;i++){//遍历当前树节点  
            if(cond_tree[i].name==t.name){//找到所有当前的商品的节点
		        leaf_node=i;
				path_num++;  
                for(int pre=cond_tree[i].parent;pre!=0;pre=cond_tree[pre].parent){//向上查找路径 
					cond_node_num++; 
                    newdatabase[new_data_size].push_back(DataNode(cond_tree[pre].name,cond_tree[i].num));//将路径上的数据加入新的数据中 
                }  
                new_data_size++;//新数据规模加1  
            } 
        }  
        
        /*若当前树仅剩一条路径，可直接将路径上所有节点组合输出，无需递归*/ 
        if(path_num==1){//仅剩一条路径 
	        if(cond_tree[leaf_node].num<minsupport)	continue;//如果当前的节点支持度小于最小支持度，直接剪枝 
	        if(cond_node_num>0)	one_path_out(newdatabase,leaf_node,cond_node_num,new_post_index);//当前节点存在父节点，之间进行全组合输出 
		}
		else	FP_Growth(newdatabase,new_post_index,new_data_size);//递归调用  
        
        /*恢复现场*/ 
        tree_sz=tmpsz;//恢复树的规模  
        for(int i=0;i<tree_sz;i++){//恢复树节点  
            cond_tree[i]=Copy(tree_buffer[i]);  
        }
    }  

}  
int main(){    
    outfile.open("out.txt");
    printf("请指定最小支持数\n");
    cin>>minsupport;
    Input();//输入数据  
    li_d post_index=li_d ();//初始化前缀列表为空 
    FP_Growth(DataBase,post_index,data_size);//递归查找 
    return 0;  
}  

