#include<iostream>
#include<set>
#include<map>
#include<string>
#include<vector>
#include<fstream>
#include<cstring>
#include<cmath> 
using namespace std;
typedef map<set<string>,int> Map;

Map Lk ;//k项集的频繁项集以及对应的数量  
vector<set<string> > DataBase;//原始数据集 
Map FP_set;//全体频繁项集合,用于剪枝时子集的查找 
vector<string> item;//记录全体商品名 
int minsupport = 50;//最小支持数 
double min_be=0.05;//最小置信度 
int flag;//辅助标记剪枝的判断 
ofstream outfile;//用于输出结果到文本文档中 
set<string> Data_Line;//辅助记录每一行的商品 


void Input(){//输入函数，用于导入实验数据,将初始化的数据保存在DataBase中
	ifstream f("./project4 - Groceries.csv");//打开文件输入流，导入数据文件 
	
	/*当数据文件打开失败的时候，直接打印错误信息，退出程序*/ 
	if(f.fail()){
		cout << "Connot open file"<<endl;
		return ;
	}
    string line; 
	string str="";//定义字符串，记录处理数据后，得到的商品名称 
	std::getline(f,line);//读取第一行数据，将第一行的条目排除 
	
	/*每次读取一行数据，并进行处理，保存在DataBase中*/ 
	while(std::getline(f,line)){
		Data_Line.clear();
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
				str+=line[l];
			}
			
			/*否则，当前商品名结束，开始保存到DataBase中*/ 
			else{
				Data_Line.insert(str);
				item.push_back(str);
				str="";
			}
			/*如果读取到'}'，当前交易记录读取结束，离开循环，进行下一条的读取*/ 
			if(line[l]=='}'){
				DataBase.push_back(Data_Line);
				break;
			}
		}
	}
}
int judge(set<string> x,set<string> y){//判断函数，用于判断两个项之间的包含关系 
  int flag=0;
  
  /*循环遍历查找，x中是否有元素不在y中*/ 
  for(set<string>::iterator it=x.begin(); it!=x.end();it++){
     if(y.find(*it)==y.end()){//x中存在元素未出现在y中 
         flag=2;//将flag从0变为2，说明至少x不是y的子集 
         break;
     }
  }
  
  /*循环遍历查找，y中是否有元素不在x中*/ 
  for( set<string>::iterator it=y.begin(); it!=y.end();it++ ){
      if( x.find(*it)==x.end()){//y中存在元素未出现在x中 
         flag+=1;//将flag加1，说明y至少不是x的子集 
		 /*如果原本flag为0,那么此时最终返回结果为1,说明x是y的真子集
		   如果原本flag为2，那么此时最终返回结果为3，说明x和y彼此没有包含关系
		   */              
         break;
      }
  }
  /*如果最终flag为0，说明x和y相等
  	如果最终flag为2，说明y是x的真子集 
    */
  return flag;
}
void L1_gen(){//生成函数，用于生成1项的频繁项 
     for(int j=0; j<item.size();j++){//扫描数据库中所有的商品 
      int flag=1;//辅助变量，用于判断商品是否存在 
      for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){//遍历所有当前的频繁项 
        if((l_it->first).find(item[j]) != (l_it->first).end()){//在频繁项中发现了当前项 
           Lk[l_it->first]++;//将对应的商品数量加1 
           flag=0;//标记为已发现 
           break;
        }
      }
     if(flag){//当前频繁项集中并不存在当前商品 
          Data_Line.clear();
          Data_Line.insert(item[j]);
          Lk.insert(pair<set<string>,int>(Data_Line,1));//在频繁项集中添加，数量设置为1 
     }
    }
    
    /*将当前频繁项集中数量小于最小支持度的从频繁项集中删去*/ 
    Map::iterator m_it=Lk.begin(); 
    while( m_it!=Lk.end()){//遍历频繁项集 
      if( m_it->second< minsupport){//不满足最小支持数的，删去 
        Lk.erase(m_it++);
       } 
      else m_it++;
    }

  /*将当前得到的一阶频繁项集输出到文本文件中去*/ 
  for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){//遍历频繁项集 
      for(set<string>::iterator s_it=(l_it->first).begin(); s_it!=(l_it->first).end(); s_it++){//遍历项集中每一件商品 
         //outfile<<*s_it<<" "<<l_it->second<<endl;//属于商品名以及数量 
      } 
  }
}
 
Map apriori_gen(Map &Lk,int k){//用于生成1阶以上的频繁项集 
   Map Lk_temp;//新生成的k阶频繁项集 
   set<string> s_temp;//保存生成的频繁项 
   if(k>2){//阶数大于2的情况 
   	   /*利用k-1阶频繁项构造k阶候选项*/   
	   for(Map::iterator l_it1=Lk.begin();l_it1!=Lk.end();l_it1++){//遍历k-1阶的频繁项集 
	      for(Map::iterator l_it2=Lk.begin();l_it2!=Lk.end();l_it2++){//遍历k-1阶的频繁项集
	         s_temp=l_it1->first;//首先将当前的前缀保存 
	         
	         /*查找当前k-1阶频繁项集中是否存在与当前s_temp具有相同前缀的频繁项*/ 
	         for(set<string>::iterator s_it2=l_it2->first.begin(),s_it1=l_it1->first.begin();s_it2!=l_it2->first.end();s_it2++,s_it1++ ){
	               if(*s_it1==*s_it2)	continue;
	               if(*s_it1!=*s_it2){//出现匹配不同的情况 
	               	  set<string>::iterator s_it3=s_it2;
	               	  s_it3++;
	     		      if(s_it3==l_it2->first.end()){//检查不同的位是否是最后一位，如果是，则生成候选项 
	     		      	s_temp.insert(*s_it2);
	     		      	
	     		      	/*检查该候选项是否已经生成过*/ 
	               		if(Lk_temp.find(s_temp)!=Lk_temp.end())   break;//已经出现过，直接跳过 
					    else{//未出现过，则先添加到频繁项集中去 
	                  	 Lk_temp.insert(pair<set<string>,int>(s_temp,0));
	               		}
	  			      }
					   else break;//如果不匹配的不是最后一位，则直接跳过 
				   }
	         }
	      }
	   }
	   
	   /*剪枝操作*/
 	   int binary[200];
	   int t; 
	   for(Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();l_it++){//遍历当前的频繁项集 
		  memset(binary,0,sizeof binary);
		  t=0;
		  binary[0]=1;
		  while(t<(pow(2,k)-2)){
			  int bit = 0;
			  s_temp=l_it->first;//拷贝当前候选项 
	     	  for(set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){//模拟二进制遍历生成子集，判断是否为频繁项
					if(binary[bit]==1){//对应二进制位为1 
				        s_temp.erase(*s_it);//轮流去掉项中的商品，生成子集
         			}
			        bit++;//检查二进制下一位
			  } 
			  flag=1;//标记子集是否出现在频繁项集中 
     		  for(Map::iterator FP_it=FP_set.begin();FP_it!=FP_set.end();FP_it++){//遍历在频繁项集中查找当前子集 
	   		     if(0 == judge(s_temp,FP_it->first)){//在频繁项集中发现子集，将标记置0
     	   		     flag=0;
	                 break;
	             }
	          }
	          if(flag){
	          	/*for(set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){
	          		cout << *s_it << "#";
				  }
				  cout << endl;
	          	for(set<string>::iterator s_it2=s_temp.begin();s_it2!=s_temp.end();s_it2++){
				 		cout << *s_it2<<","; 
				 } 
					 cout<< endl;*/
			    break;//未发现，跳出循环
			}
	          /*模拟二进制进位*/ 
	          binary[0]++;//末位加1 
			  int b=0;
	 	      while(binary[b]>1){//出现大于1，执行进位 
				 binary[b]=0;
	             b++;
	             binary[b]++;
			   }
			   t++;
         }
	     if(flag) Lk_temp.erase(l_it);//存在子集不是频繁项集，直接剪枝 
	    } 
	    //if(Lk_temp.empty())	cout << "3333"<<endl;
	   
   } 
   else{//2阶频繁集的生成 
   		for( Map::iterator l_it1=Lk.begin();l_it1!=Lk.end();l_it1++ ){//遍历1阶频繁项集 
      		 for( Map::iterator l_it2=Lk.begin();l_it2!=Lk.end();l_it2++ ){//遍历1阶频繁项集  
		         if(!((l_it1->first > l_it2->first)||(l_it1->first < l_it2->first)))  continue;//如果两项相同则跳过，否则可直接组装 
				 string temp=*(l_it2->first.begin());//取出商品名 
				 s_temp=l_it1->first; 
				 s_temp.insert(temp);//组装为2阶候选项 
				 if(Lk_temp.find(s_temp)!=Lk_temp.end())  continue;//判断当前2阶项是否已经生成过了  
     			 else{//没有生成过，先放入频繁项集中 
         		 		Lk_temp.insert(pair<set<string>,int >(s_temp,0));  
   				  }  
		      }  
	    }  
   }
   /*开始进行当前候选集的计数*/ 
      for(Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();l_it++){//遍历候选集 
           for(int i=0;i<DataBase.size();i++){//遍历所有的原始数据项 
          		 if(judge(DataBase[i],l_it->first)%2==0){//在原始数据项中发现当前候选项，则将对应频数加1 
                  	 Lk_temp[l_it->first]++;
    		     }
		    }
		}
		
		/*遍历，将候选集中数量小于最小支持数的项删去*/ 
 	   for( Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();){//遍历候选集 
		//cout<<num<<endl;
         if(Lk_temp[l_it->first] < minsupport){//小于最小支持数，删去 
             Lk_temp.erase(l_it++);
             if(Lk_temp.empty())	break;//当前候选集为空，退出 
         }
         else l_it++;
     } 
     /*遍历频繁项集，输出结果，用于调试*/ 
	   /*for( Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();l_it++ ){//遍历频繁项集 
	        for( set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){//遍历商品名，输出 
	              //outfile<<*s_it<<"  ";
	             //cout << *s_it<<" ";
	         }
	        //outfile<<l_it->second<<endl;//输出数量 
	        //cout<<l_it->second<<endl;//输出数量 
	   
	   }*/ 
   return Lk_temp;
}
void result(int k){//根据频繁项集搜索出满足最小置信度的关联关系 
	 int binary[200];//保存模拟的二进制数 
     int t; 
     int totalnum;//定义整型，用于保存频繁项集的总数量 
	 for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){//遍历当前的频繁项集 
		  totalnum=l_it->second;//获取当前频繁项集的总数量 
		  memset(binary,0,sizeof binary);
		  t=0;
		  binary[0]=1;
		  
		  while(t<(pow(2,k-1)-2)){
		  	set<string> s_temp;
		    set<string> s_temp2;
			  int bit = 0;
			  s_temp=l_it->first;//拷贝当前候选项 
			  
	     	  for(set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){//模拟二进制遍历生成子集，判断是否为频繁项
					if(binary[bit]==1){//对应二进制位为1 
				        s_temp.erase(*s_it);//轮流去掉项中的商品，生成子集
				        s_temp2.insert(*s_it);//保存子集的补集 
         			}
			        bit++;//检查二进制下一位
			  } 
			  Map::iterator M_it;
			  M_it=FP_set.find(s_temp);//在全体频繁项集中找到子集 
			  int partnum=M_it->second;//获取子集的数量 
			  double believe = 1.0*totalnum/partnum;//计算置信度 
			  if(believe>min_be){//置信度满足条件，进行输出 
			  	for(set<string>::iterator s_it=s_temp.begin();s_it!=s_temp.end();s_it++){//打印子集 
				  //cout<<*s_it<<" ";
				  outfile<<*s_it<<"  "; 
	  	        }
			  	outfile << " ====>";
			  	for(set<string>::iterator s_it=s_temp2.begin();s_it!=s_temp2.end();s_it++){//打印补集 
				  //cout<<*s_it<<"   ";
			  	  outfile<<*s_it<<"  ";
	  	        }
	  	        //cout << believe << endl;//输出置信度 
	  	        outfile<<believe<<endl;
			  }
	          /*模拟二进制进位*/ 
	          binary[0]++;//末位加1 
			  int b=0;
	 	      while(binary[b]>1){//出现大于1，执行进位 
				 binary[b]=0;
	             b++;
	             binary[b]++;
			   }
			   t++;
            }
	    }
}
int main()
{ 
    printf("请指定最小支持数（建议50左右）\n");
	cin>>minsupport;
	printf("请指定最小置信度（建议0.1左右）\n");
	cin>>min_be; 
    outfile.open("out.txt");//打开输出文件 
	Input();//导入数据 
	L1_gen();//生成1阶项集 
  	int k=2;//当前阶数为2 
  	Map new_Lk;
  	while(true){//循环，生成k阶频繁项集 
	     
	 	 for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){
			  FP_set.insert(*l_it);
	 	 }
   	        new_Lk=apriori_gen(Lk,k);
  			if(new_Lk.empty())	break;
  			else {
  				k++;
  				Lk=new_Lk;
			  }
	}
	   
    result(k);
	return 0;
}

