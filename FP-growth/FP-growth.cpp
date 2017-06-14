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
 
 
  
  
class DataNode{//�������ݽڵ㣬���ڼ�¼ÿ����Ʒ�����ƺʹ���   
   public:
    int name;//����������nameӳ����Ʒ���Ƶ��ַ��� 
    int num; //���ڼ�¼��Ʒ������ 
    DataNode(int x,int y){//�Զ������ݽڵ�����ɺ��� 
        name=x;  
        num=y;  
    }  
};  
class TreeNode{//����FP-tree�����ڵ���   
   public:
    int parent;//���ڵ�Ľڵ��� 
    vector<int>child;//�ӽڵ�Ľڵ��� 
    int name;//�ڵ���Ʒ�����Ʊ�� 
	int num;//�ڵ���Ʒ������ 
}; 
TreeNode Copy(TreeNode tmp){//�������ڵ�Ŀ�������������֮����еݹ��״̬����ͻָ�   
    TreeNode orginal;//ԭʼ�ڵ�  
    orginal.parent=tmp.parent;//�������ڵ� 
    orginal.name=tmp.name;//�����ڵ���Ʒ�����Ʊ��  
	orginal.num=tmp.num;//�����ڵ���Ʒ������  
    return orginal;  
} 
typedef list<DataNode>  li_d;  
int data_size=9835;//���ݵ����� 
int minsupport=50;//��С֧���� 
int tree_sz;//FP���Ľڵ�����  
int total;//��������Ʒ�������� 
int count[10000];//ÿ����Ʒ������  
int weight[10000];//��Ʒ��Ȩ�����飬���ڽ���֮���ÿ��������������
int binary[200];
int path_num; 
int cond_node_num;
ofstream outfile;
int dictionary[100000][200];//�����ṹ�ֵ仯�����ڼ�¼ÿ�����ڵ���ӽڵ㣬���ٱ������ռ任ʱ�� 
map<string,int> index;//������Ʒ���ƺ���Ʒ��ŵ�ӳ�� 
li_d DataBase[10000];//���ݿ⣬���ڼ�¼ÿ�ֽ��������� 
TreeNode *cond_tree=new TreeNode[50000];//��¼FPtree�ṹ 

string getname(int item){//���룺��Ʒ��ţ��������Ʒ���Ƶ��ַ���  
    string s;//�����ַ�����������Ʒ����
	/*ѭ������index��������Ʒ�ı�ŵ�ƥ�䣬��ԭ��Ʒ��*/  
    for(map<string,int>::iterator it=index.begin();it!=index.end();it++)  
    {  
        if(it->second==item)//�����������ƥ��ı�ţ��ͽ���Ӧ����Ʒ���ַ���ƥ����� 
        {  
            s=it->first;  
            return s;  
        }  
    }
    
	s="This is something wrong with index,we can't find it.";//���δ�ҵ���˵��index���ִ����ύ������Ϣ  
    return s;  
}  
void Input(){//���뺯�������ڵ���ʵ������,����ʼ�������ݱ�����DataBase������  
	ifstream f("./project4 - Groceries - 2.csv");//���ļ������������������ļ� 
	
	/*�������ļ���ʧ�ܵ�ʱ��ֱ�Ӵ�ӡ������Ϣ���˳�����*/ 
	if(f.fail()){
		cout << "Connot open file"<<endl;
		return;
	}
	
	string line;//�����ַ�������������ԭʼ���� 
	int num=1;//����������ͣ���¼��Ʒ�ı��� 
	string s="";//�����ַ�������¼�������ݺ󣬵õ�����Ʒ���� 
	std::getline(f,line);//��ȡ��һ�����ݣ�����һ�е���Ŀ�ų� 
	int i = 0;//��¼��ǰ��ȡ���ݵ�����������DataBase��������
	
	/*ÿ�ζ�ȡһ�����ݣ������д���������DataBase��*/ 
	while(std::getline(f,line)){
		i++;//ÿ�ζ�ȡ������һ 
		bool flag = true;//���岼���ͣ������ж��Ƿ����'{','{'����Ʒ���ƵĿ�ʼ 
		for(int l = 0; l < line.length();l++){
			if(flag){//flagΪtrue˵����δ������Ʒ����,Ϊfalse˵��������Ʒ���ƣ���ʼ������Ʒ�� 
		       if(line[l]!='{')	continue;//û�ж���'{'������flag״̬���������²��� 
   			   else {//����'{'���ı�flag״̬ 
				  flag = false;
				  continue;
	  		    }
			}
			/*û�ж���','����'}'˵����ǰ��Ʒ����δ����������s���ӳ�*/ 
			if(line[l]!=','&& line[l]!='}'){
				s+=line[l];
			}
			
			/*���򣬵�ǰ��Ʒ����������ʼ���浽DataBase��*/ 
			else{
				map<string,int>::iterator index_it;//��������������ڱ�����Ʒ���Ʋ��ҵ�λ��  
            	index_it=index.find(s);//��index�в��ҵ�ǰ����Ʒ����
				  
            	if(index_it==index.end()){//������������index��β��˵��δ�ҵ���ǰ���ƣ���Ҫ�½�ӳ�� 
	                index[s]=num;//�����µ�ӳ���ϵ  
	                num++;//��Ʒ��ż�1  
            	}  
             	DataBase[i].push_back(DataNode(index[s],1));//����ǰ�ı�ź�����1������DataBase��ǰ��¼��    
				s="";//����ַ���s��������һ����Ʒ���Ƶļ�¼ 
			}
			/*�����ȡ��'}'����ǰ���׼�¼��ȡ�������뿪ѭ����������һ���Ķ�ȡ*/ 
			if(line[l]=='}'){
				break;
			}
		}
	}     
	total = num;
	//printf("%d\n",total);//��ӡ��Ʒ����������������ڵ��ԣ���ֹ������� 
}  


bool cmp(DataNode a,DataNode b){//����ȽϺ������Ƚ����ݽڵ����Ʒ����������headtable������
    return a.num>=b.num;  
}  

 
li_d  CreateHeadTable(li_d *database,int datasize){//�������ڴ�������ͷ��   
    memset(count,0,sizeof(count));//��ռ������飬׼����¼ÿ����Ʒ������  
    li_d headtable;//�������ݽڵ��б����ڼ�¼headtable
	 
	/*����ÿһ�����������е�ÿһ����Ʒ��ͳ��ÿ����Ʒ��������*/ 
    for(int i=0;i<datasize;i++)
    {  
           
         for(li_d ::iterator iter=database[i].begin();iter!=database[i].end();iter++)//����ÿһ�����׼�¼��ÿ����Ʒ  
         {    
             count[iter->name]+=iter->num;//����ǰ��Ʒ����Ŀ��1 
         }  
    } 
	
	/*����ÿһ����Ʒ��������������С֧��������Ʒɸѡ����*/ 
    for(int i=0;i<total;i++)  
    {  
        if(count[i]<minsupport) continue;  
        headtable.push_back(DataNode(i,count[i]));//�����Ʒ��������С֧�������������ݼ��뵽headtable�� 
    }  
    headtable.sort(cmp);//������Ʒ������headtable�е�ÿһ��������򣬴Ӵ�С 
	return headtable;
}


bool cmp2(DataNode x,DataNode y){//����ȽϺ��������ڶ�ÿ�����׼�¼����Ʒ�����ڲ�����  
    if(weight[x.name]<=weight[y.name])
        return true;  
    return false;  
}  
void sort_weight(li_d *database,li_d headtable,int datasize){//��weight���ݽ��й����������ø������ÿ�����׼�¼����Ʒ�����ڲ����� 
	
    memset(weight,0,sizeof(weight));//���Ȩ�����飬�����ڲ�����  
    int order=0;//��ʼ��Ȩ�ر��
	  
	/*������ͷ����Ȩ�����ݽ��й���*/  
    for(li_d ::iterator iter=headtable.begin();iter!=headtable.end();iter++){    
        weight[iter->name]=order;//����ǰȨ�ظ�����Ӧ��Ʒ 
        order++; //Ȩ�ر��˳�μ�1 
    } 
	
 	/*�������ݿ⣬�����ݿ��е�ÿһ�����׼�¼���м�飬�޳�С����С֧�����ģ��ӿ���� */
    for(int i=0;i<datasize;i++){//�������ݿ�  
         for(li_d ::iterator iter=database[i].begin();iter!=database[i].end();){//����ÿһ�����׼�¼   
             if(count[iter->name]<minsupport){//�����ǰ��Ʒ������С����С֧�ֶȣ��ͽ���ӽ��׼�¼��ɾ��  
                 database[i].erase(iter++);  
             }  
             else  
            {  
                iter++;  
            }  
         }  
         database[i].sort(cmp2);//����cmp2,�Ե�ǰ�Ľ��׼�¼�������� 
    }  
}
 
TreeNode* BuildTree(li_d a [],int num,TreeNode *FP_tree){//���õ�ǰ�������б����FP���Ĺ���  
    //TreeNode *Val=new TreeNode[100];//������洢���ڵ㣬����ͨ��ÿ���ڵ��parent��child��ԭ��������Val[0]����ڵ�  
    memset(dictionary,0,sizeof(dictionary));//���dictionary���飬���ڼ�¼��ǰ�Ľڵ㸸�ӹ�ϵ  
    int node_num=1;//��¼��ǰFP���Ľڵ���  
    
	/*������ǰ���ݣ��������ṹ*/ 
	for(int i=0;i<num;i++){//������ǰ���ݵ�ÿһ��  
        int root=0;//ÿ����¼�Ĺ�������null�ڵ㿪ʼ 
        for(li_d ::iterator it=a[i].begin();it!=a[i].end();it++){//����ÿһ�����׼�¼  
            DataNode t=*it;  
            int s=t.name;
			
			/*�������������Ʒ���ǵ�ǰ�ڵ���ӽڵ㣬�����ӽڵ�*/  
            if(dictionary[root][s]==0){  
                memset(dictionary[node_num],0,sizeof(dictionary[node_num]));//��յ�ǰ�������dictionary�б����ڼ�¼�����ӽڵ�  
                TreeNode tmp=TreeNode();//�½��ڵ�  
                tmp.num=t.num;//��ֵ����  
                tmp.name=s;  
                tmp.parent=root;//���ڵ㶨��Ϊ��ǰ�ڵ�  
                FP_tree[node_num]=tmp;//�����ڵ㸳ֵ����ǰ�ڵ��ŵ����ڵ�  
                FP_tree[root].child.push_back(node_num);//�ڵ�ǰ�����ڵ���ӽڵ��б�������������ڵ� 
                dictionary[root][s]=node_num;//���µ�ǰ�ڵ��dictionary���ݣ�����֮��Ĳ��Ҳ��� 
                node_num++;//�ڵ����ݼ�1 
            }
			
			/*����������Ʒ�ǵ�ǰ�ڵ���ӽڵ㣬��FP���ж�Ӧ�ڵ��������1*/  
            else{  
                for(int k=0;k<FP_tree[root].child.size();k++){  
                    if(FP_tree[FP_tree[root].child[k]].name==s){//���ҵ���Ӧ��Ʒ���ӽڵ�  
                        FP_tree[FP_tree[root].child[k]].num+=t.num;//����Ʒ�������� 
                         break;  
                    }  
                }  
            }  
            root=dictionary[root][s];//����ǰ�ڵ���·�������ƶ� 
        }  
    }  
    tree_sz=node_num;//�������Ĵ�С 
    return FP_tree; 
}  

void one_path_out(li_d database[],int leaf_node,int cond_code_num,li_d  post_index){//���ڵ�ǰ��ֻʣ��һ��·��ʱ��ֱ�����·���ϵ�ȫ��������ϣ����ٵݹ� 
	//cout <<"cond_code_num"<<cond_code_num<<endl;
	string str="";//�����ַ��������ڼ�¼��ǰ��ǰ׺ 
	if(!post_index.empty()){//ǰ׺��Ϊ��    
            for(li_d ::iterator it=post_index.begin();it!=post_index.end();it++){//ѭ���õ�ǰ׺��    
                str+=getname(it->name)+" "; 
            }      
    }  
	memset(binary,0,sizeof(binary));//���binary���飬����ģ������ƽ�λ 
	int bit=0;//�����������ڿ���ѭ������
	
	/*ģ������ƽ�λ���̣�����·���ڵ���ϵ�ö��*/ 
	while(bit<pow(2,cond_code_num)){//ѭ������Ӧ����2��cond_code_num�η� 
		bit++;
		binary[0]++;//������ĩλ��1 
		string s="";//�����ַ������ڵ�ǰ��¼��ǰ��Ƶ���� 
		int j = 0;//�������ͣ���������binary���� 
		int num = cond_tree[leaf_node].num;//��ȡ��ǰƵ�� 
		for(li_d ::iterator iter=database[0].begin();iter!=database[0].end();iter++){
			if(binary[j]==1)	s+=getname(iter->name)+" ";//�ַ������ϵ�ǰ������λΪ1�Ķ�Ӧ����Ʒ 
			j++;//����������һλ 
		}
		s+=str;//����ǰ׺�� 
	    outfile<<s<< num << endl;//��� 
		int i = 0;//�������ͣ����ڽ�λ�ƶ� 
		while(binary[i]>1){//��鵱ǰλ�Ƿ����2 
			binary[i]=0;//����2�ͽ���ǰλ��Ϊ0 
			i++;//ǰ��һλ 
			binary[i]+=1;//��ǰλ��1 
		}
	}
	
}
void FP_Growth(li_d database[],li_d post_index,int data_size){//ͨ���ݹ飬ʵ��FP-growth�㷨����FP����Ѱ��Ƶ���� 
    li_d head_table=CreateHeadTable(database,data_size);//���ú���CreateHeadTable���������ͷ 
    sort_weight(database,head_table,data_size);//��ÿ�����׼�¼�ڲ��������� 
    BuildTree(database,data_size,cond_tree);//����FP�� 
    TreeNode *tree_buffer=new TreeNode[50000];//����FPtree��buffer���飬�����ڵݹ��м�¼FP�����ڵݹ�����ǻ�ԭ 
	int tmpsz=tree_sz;//��¼��ǰFP���Ĺ�ģ������FP���ָ�ʱ��ѭ���ı߽� 
    
	/*����FP��*/ 
	for(int i=0;i<tree_sz;i++){  
        tree_buffer[i]=Copy(cond_tree[i]);  
    }
	
	/*�����ǰ���������ֻ��һ�����ڵ㣬˵�����ṹΪ�գ��ݹ����*/  
    if(tree_sz==1){  
        return;  
    }
	/*����head_table���Ե�ǰ��Ƶ����������,��Ϊ���ҹ�����ϵ����ǰ׺��û��Ԫ�ص�ʱ��Ƶ��������*/ 
    if(!post_index.empty()){  
        for(li_d ::iterator iter=head_table.begin();iter!=head_table.end();iter++){//����head_table  
            DataNode t=*iter;  
            string str=getname(t.name);
            int num=t.num;
            for(li_d ::iterator it=post_index.begin();it!=post_index.end();it++){//ѭ�����ǰ׺��    
                str+=","+getname(it->name); 
            }
            outfile<<str<<" "<<num<<endl;  
        }  
    }  
    
    /*����������С�����˳Ѱ����head_table���б�����������Ҷ�ڵ㿪ʼ��������Ϊǰ׺�����ϲ��ң���������FPtree*/ 
    for(li_d ::reverse_iterator iter=head_table.rbegin();iter!=head_table.rend();iter++){  
        DataNode t=*iter;  
        li_d new_post_index;//�����µ�ǰ׺�б� 
        new_post_index.push_back(DataNode(t.name,t.num));//����ǰ�����ڵ���뵽ǰ׺�б��� 
        if(!post_index.empty()){//��ǰǰ׺�б�ǿգ�Ҫ���µ��б���кϲ�����  
            li_d tmp=post_index;  
            new_post_index.merge(post_index,cmp);  
            post_index=tmp;  
        }  
        li_d newdatabase[10000];//�������ݽڵ��б�������һ�ֵݹ�����ݱ��� 
        int new_data_size=0;
        path_num=0;
        cond_node_num=0;
        int leaf_node;
		/*ѭ��������ǰ�����ڵ㣬�����е����е�ǰ������Ʒ�ҳ��������ϲ������й���·���������µ�����*/  
        for(int i=0;i<tree_sz;i++){//������ǰ���ڵ�  
            if(cond_tree[i].name==t.name){//�ҵ����е�ǰ����Ʒ�Ľڵ�
		        leaf_node=i;
				path_num++;  
                for(int pre=cond_tree[i].parent;pre!=0;pre=cond_tree[pre].parent){//���ϲ���·�� 
					cond_node_num++; 
                    newdatabase[new_data_size].push_back(DataNode(cond_tree[pre].name,cond_tree[i].num));//��·���ϵ����ݼ����µ������� 
                }  
                new_data_size++;//�����ݹ�ģ��1  
            } 
        }  
        
        /*����ǰ����ʣһ��·������ֱ�ӽ�·�������нڵ�������������ݹ�*/ 
        if(path_num==1){//��ʣһ��·�� 
	        if(cond_tree[leaf_node].num<minsupport)	continue;//�����ǰ�Ľڵ�֧�ֶ�С����С֧�ֶȣ�ֱ�Ӽ�֦ 
	        if(cond_node_num>0)	one_path_out(newdatabase,leaf_node,cond_node_num,new_post_index);//��ǰ�ڵ���ڸ��ڵ㣬֮�����ȫ������ 
		}
		else	FP_Growth(newdatabase,new_post_index,new_data_size);//�ݹ����  
        
        /*�ָ��ֳ�*/ 
        tree_sz=tmpsz;//�ָ����Ĺ�ģ  
        for(int i=0;i<tree_sz;i++){//�ָ����ڵ�  
            cond_tree[i]=Copy(tree_buffer[i]);  
        }
    }  

}  
int main(){    
    outfile.open("out.txt");
    printf("��ָ����С֧����\n");
    cin>>minsupport;
    Input();//��������  
    li_d post_index=li_d ();//��ʼ��ǰ׺�б�Ϊ�� 
    FP_Growth(DataBase,post_index,data_size);//�ݹ���� 
    return 0;  
}  

