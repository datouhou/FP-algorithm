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

Map Lk ;//k���Ƶ����Լ���Ӧ������  
vector<set<string> > DataBase;//ԭʼ���ݼ� 
Map FP_set;//ȫ��Ƶ�����,���ڼ�֦ʱ�Ӽ��Ĳ��� 
vector<string> item;//��¼ȫ����Ʒ�� 
int minsupport = 50;//��С֧���� 
double min_be=0.05;//��С���Ŷ� 
int flag;//������Ǽ�֦���ж� 
ofstream outfile;//�������������ı��ĵ��� 
set<string> Data_Line;//������¼ÿһ�е���Ʒ 


void Input(){//���뺯�������ڵ���ʵ������,����ʼ�������ݱ�����DataBase��
	ifstream f("./project4 - Groceries.csv");//���ļ������������������ļ� 
	
	/*�������ļ���ʧ�ܵ�ʱ��ֱ�Ӵ�ӡ������Ϣ���˳�����*/ 
	if(f.fail()){
		cout << "Connot open file"<<endl;
		return ;
	}
    string line; 
	string str="";//�����ַ�������¼�������ݺ󣬵õ�����Ʒ���� 
	std::getline(f,line);//��ȡ��һ�����ݣ�����һ�е���Ŀ�ų� 
	
	/*ÿ�ζ�ȡһ�����ݣ������д���������DataBase��*/ 
	while(std::getline(f,line)){
		Data_Line.clear();
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
				str+=line[l];
			}
			
			/*���򣬵�ǰ��Ʒ����������ʼ���浽DataBase��*/ 
			else{
				Data_Line.insert(str);
				item.push_back(str);
				str="";
			}
			/*�����ȡ��'}'����ǰ���׼�¼��ȡ�������뿪ѭ����������һ���Ķ�ȡ*/ 
			if(line[l]=='}'){
				DataBase.push_back(Data_Line);
				break;
			}
		}
	}
}
int judge(set<string> x,set<string> y){//�жϺ����������ж�������֮��İ�����ϵ 
  int flag=0;
  
  /*ѭ���������ң�x���Ƿ���Ԫ�ز���y��*/ 
  for(set<string>::iterator it=x.begin(); it!=x.end();it++){
     if(y.find(*it)==y.end()){//x�д���Ԫ��δ������y�� 
         flag=2;//��flag��0��Ϊ2��˵������x����y���Ӽ� 
         break;
     }
  }
  
  /*ѭ���������ң�y���Ƿ���Ԫ�ز���x��*/ 
  for( set<string>::iterator it=y.begin(); it!=y.end();it++ ){
      if( x.find(*it)==x.end()){//y�д���Ԫ��δ������x�� 
         flag+=1;//��flag��1��˵��y���ٲ���x���Ӽ� 
		 /*���ԭ��flagΪ0,��ô��ʱ���շ��ؽ��Ϊ1,˵��x��y�����Ӽ�
		   ���ԭ��flagΪ2����ô��ʱ���շ��ؽ��Ϊ3��˵��x��y�˴�û�а�����ϵ
		   */              
         break;
      }
  }
  /*�������flagΪ0��˵��x��y���
  	�������flagΪ2��˵��y��x�����Ӽ� 
    */
  return flag;
}
void L1_gen(){//���ɺ�������������1���Ƶ���� 
     for(int j=0; j<item.size();j++){//ɨ�����ݿ������е���Ʒ 
      int flag=1;//���������������ж���Ʒ�Ƿ���� 
      for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){//�������е�ǰ��Ƶ���� 
        if((l_it->first).find(item[j]) != (l_it->first).end()){//��Ƶ�����з����˵�ǰ�� 
           Lk[l_it->first]++;//����Ӧ����Ʒ������1 
           flag=0;//���Ϊ�ѷ��� 
           break;
        }
      }
     if(flag){//��ǰƵ����в������ڵ�ǰ��Ʒ 
          Data_Line.clear();
          Data_Line.insert(item[j]);
          Lk.insert(pair<set<string>,int>(Data_Line,1));//��Ƶ�������ӣ���������Ϊ1 
     }
    }
    
    /*����ǰƵ���������С����С֧�ֶȵĴ�Ƶ�����ɾȥ*/ 
    Map::iterator m_it=Lk.begin(); 
    while( m_it!=Lk.end()){//����Ƶ��� 
      if( m_it->second< minsupport){//��������С֧�����ģ�ɾȥ 
        Lk.erase(m_it++);
       } 
      else m_it++;
    }

  /*����ǰ�õ���һ��Ƶ���������ı��ļ���ȥ*/ 
  for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){//����Ƶ��� 
      for(set<string>::iterator s_it=(l_it->first).begin(); s_it!=(l_it->first).end(); s_it++){//�������ÿһ����Ʒ 
         //outfile<<*s_it<<" "<<l_it->second<<endl;//������Ʒ���Լ����� 
      } 
  }
}
 
Map apriori_gen(Map &Lk,int k){//��������1�����ϵ�Ƶ��� 
   Map Lk_temp;//�����ɵ�k��Ƶ��� 
   set<string> s_temp;//�������ɵ�Ƶ���� 
   if(k>2){//��������2����� 
   	   /*����k-1��Ƶ�����k�׺�ѡ��*/   
	   for(Map::iterator l_it1=Lk.begin();l_it1!=Lk.end();l_it1++){//����k-1�׵�Ƶ��� 
	      for(Map::iterator l_it2=Lk.begin();l_it2!=Lk.end();l_it2++){//����k-1�׵�Ƶ���
	         s_temp=l_it1->first;//���Ƚ���ǰ��ǰ׺���� 
	         
	         /*���ҵ�ǰk-1��Ƶ������Ƿ�����뵱ǰs_temp������ͬǰ׺��Ƶ����*/ 
	         for(set<string>::iterator s_it2=l_it2->first.begin(),s_it1=l_it1->first.begin();s_it2!=l_it2->first.end();s_it2++,s_it1++ ){
	               if(*s_it1==*s_it2)	continue;
	               if(*s_it1!=*s_it2){//����ƥ�䲻ͬ����� 
	               	  set<string>::iterator s_it3=s_it2;
	               	  s_it3++;
	     		      if(s_it3==l_it2->first.end()){//��鲻ͬ��λ�Ƿ������һλ������ǣ������ɺ�ѡ�� 
	     		      	s_temp.insert(*s_it2);
	     		      	
	     		      	/*���ú�ѡ���Ƿ��Ѿ����ɹ�*/ 
	               		if(Lk_temp.find(s_temp)!=Lk_temp.end())   break;//�Ѿ����ֹ���ֱ������ 
					    else{//δ���ֹ���������ӵ�Ƶ�����ȥ 
	                  	 Lk_temp.insert(pair<set<string>,int>(s_temp,0));
	               		}
	  			      }
					   else break;//�����ƥ��Ĳ������һλ����ֱ������ 
				   }
	         }
	      }
	   }
	   
	   /*��֦����*/
 	   int binary[200];
	   int t; 
	   for(Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();l_it++){//������ǰ��Ƶ��� 
		  memset(binary,0,sizeof binary);
		  t=0;
		  binary[0]=1;
		  while(t<(pow(2,k)-2)){
			  int bit = 0;
			  s_temp=l_it->first;//������ǰ��ѡ�� 
	     	  for(set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){//ģ������Ʊ��������Ӽ����ж��Ƿ�ΪƵ����
					if(binary[bit]==1){//��Ӧ������λΪ1 
				        s_temp.erase(*s_it);//����ȥ�����е���Ʒ�������Ӽ�
         			}
			        bit++;//����������һλ
			  } 
			  flag=1;//����Ӽ��Ƿ������Ƶ����� 
     		  for(Map::iterator FP_it=FP_set.begin();FP_it!=FP_set.end();FP_it++){//������Ƶ����в��ҵ�ǰ�Ӽ� 
	   		     if(0 == judge(s_temp,FP_it->first)){//��Ƶ����з����Ӽ����������0
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
			    break;//δ���֣�����ѭ��
			}
	          /*ģ������ƽ�λ*/ 
	          binary[0]++;//ĩλ��1 
			  int b=0;
	 	      while(binary[b]>1){//���ִ���1��ִ�н�λ 
				 binary[b]=0;
	             b++;
	             binary[b]++;
			   }
			   t++;
         }
	     if(flag) Lk_temp.erase(l_it);//�����Ӽ�����Ƶ�����ֱ�Ӽ�֦ 
	    } 
	    //if(Lk_temp.empty())	cout << "3333"<<endl;
	   
   } 
   else{//2��Ƶ���������� 
   		for( Map::iterator l_it1=Lk.begin();l_it1!=Lk.end();l_it1++ ){//����1��Ƶ��� 
      		 for( Map::iterator l_it2=Lk.begin();l_it2!=Lk.end();l_it2++ ){//����1��Ƶ���  
		         if(!((l_it1->first > l_it2->first)||(l_it1->first < l_it2->first)))  continue;//���������ͬ�������������ֱ����װ 
				 string temp=*(l_it2->first.begin());//ȡ����Ʒ�� 
				 s_temp=l_it1->first; 
				 s_temp.insert(temp);//��װΪ2�׺�ѡ�� 
				 if(Lk_temp.find(s_temp)!=Lk_temp.end())  continue;//�жϵ�ǰ2�����Ƿ��Ѿ����ɹ���  
     			 else{//û�����ɹ����ȷ���Ƶ����� 
         		 		Lk_temp.insert(pair<set<string>,int >(s_temp,0));  
   				  }  
		      }  
	    }  
   }
   /*��ʼ���е�ǰ��ѡ���ļ���*/ 
      for(Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();l_it++){//������ѡ�� 
           for(int i=0;i<DataBase.size();i++){//�������е�ԭʼ������ 
          		 if(judge(DataBase[i],l_it->first)%2==0){//��ԭʼ�������з��ֵ�ǰ��ѡ��򽫶�ӦƵ����1 
                  	 Lk_temp[l_it->first]++;
    		     }
		    }
		}
		
		/*����������ѡ��������С����С֧��������ɾȥ*/ 
 	   for( Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();){//������ѡ�� 
		//cout<<num<<endl;
         if(Lk_temp[l_it->first] < minsupport){//С����С֧������ɾȥ 
             Lk_temp.erase(l_it++);
             if(Lk_temp.empty())	break;//��ǰ��ѡ��Ϊ�գ��˳� 
         }
         else l_it++;
     } 
     /*����Ƶ����������������ڵ���*/ 
	   /*for( Map::iterator l_it=Lk_temp.begin();l_it!=Lk_temp.end();l_it++ ){//����Ƶ��� 
	        for( set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){//������Ʒ������� 
	              //outfile<<*s_it<<"  ";
	             //cout << *s_it<<" ";
	         }
	        //outfile<<l_it->second<<endl;//������� 
	        //cout<<l_it->second<<endl;//������� 
	   
	   }*/ 
   return Lk_temp;
}
void result(int k){//����Ƶ���������������С���ŶȵĹ�����ϵ 
	 int binary[200];//����ģ��Ķ������� 
     int t; 
     int totalnum;//�������ͣ����ڱ���Ƶ����������� 
	 for(Map::iterator l_it=Lk.begin();l_it!=Lk.end();l_it++){//������ǰ��Ƶ��� 
		  totalnum=l_it->second;//��ȡ��ǰƵ����������� 
		  memset(binary,0,sizeof binary);
		  t=0;
		  binary[0]=1;
		  
		  while(t<(pow(2,k-1)-2)){
		  	set<string> s_temp;
		    set<string> s_temp2;
			  int bit = 0;
			  s_temp=l_it->first;//������ǰ��ѡ�� 
			  
	     	  for(set<string>::iterator s_it=l_it->first.begin();s_it!=l_it->first.end();s_it++ ){//ģ������Ʊ��������Ӽ����ж��Ƿ�ΪƵ����
					if(binary[bit]==1){//��Ӧ������λΪ1 
				        s_temp.erase(*s_it);//����ȥ�����е���Ʒ�������Ӽ�
				        s_temp2.insert(*s_it);//�����Ӽ��Ĳ��� 
         			}
			        bit++;//����������һλ
			  } 
			  Map::iterator M_it;
			  M_it=FP_set.find(s_temp);//��ȫ��Ƶ������ҵ��Ӽ� 
			  int partnum=M_it->second;//��ȡ�Ӽ������� 
			  double believe = 1.0*totalnum/partnum;//�������Ŷ� 
			  if(believe>min_be){//���Ŷ������������������ 
			  	for(set<string>::iterator s_it=s_temp.begin();s_it!=s_temp.end();s_it++){//��ӡ�Ӽ� 
				  //cout<<*s_it<<" ";
				  outfile<<*s_it<<"  "; 
	  	        }
			  	outfile << " ====>";
			  	for(set<string>::iterator s_it=s_temp2.begin();s_it!=s_temp2.end();s_it++){//��ӡ���� 
				  //cout<<*s_it<<"   ";
			  	  outfile<<*s_it<<"  ";
	  	        }
	  	        //cout << believe << endl;//������Ŷ� 
	  	        outfile<<believe<<endl;
			  }
	          /*ģ������ƽ�λ*/ 
	          binary[0]++;//ĩλ��1 
			  int b=0;
	 	      while(binary[b]>1){//���ִ���1��ִ�н�λ 
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
    printf("��ָ����С֧����������50���ң�\n");
	cin>>minsupport;
	printf("��ָ����С���Ŷȣ�����0.1���ң�\n");
	cin>>min_be; 
    outfile.open("out.txt");//������ļ� 
	Input();//�������� 
	L1_gen();//����1��� 
  	int k=2;//��ǰ����Ϊ2 
  	Map new_Lk;
  	while(true){//ѭ��������k��Ƶ��� 
	     
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

