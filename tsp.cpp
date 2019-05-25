#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>

double distance[10][11];//城市之间的距离 
int dai,die;
int cities[2][10];//记录城市坐标
int citynum=10;
int groupbest[11];//最优解染色体 
double groupbestp;//最优解的p 
double groupbestfit;//最优解的fit 
int changebest;//要不要用最优解替代新种群 

struct group
{
  int city[11];//一维记录城市序号，二三维记录坐标
  double p;//占总群的概率 
  double fit;//适应度 
  double jileigailv;
}group[10];

/*用来计算种群的p、fit*/
void jisuan() 
{
  int i,j,k;
  double ss,s;
  s=0.0;
  ss=0.0;
  for(k=0;k<10;k++)
  {
    for(i=0;i<citynum;i++)
        {
          s+=distance[group[k].city[i]][group[k].city[i+1]];
        }
    group[k].fit=1.0/s;
    ss+=group[k].fit;
  }
  s=0.0;
  for(i=0;i<10;i++)
  {
  group[i].p=group[i].fit/ss;
  s+=group[i].p;
  group[i].jileigailv=s;
  } 
}
/*保存最优解*/ 
void savebest()
{
  int i,j,flag=0;
  double fit=groupbestfit;
  j=0;
  for(i=0;i<10;i++)
  {
     if(group[i].fit>fit)
     {
       j=i;
       fit=group[i].fit;
       flag=1;//标记已经有更好的 
     }
  }
     if(flag)
     {
         dai=die;
       for(i=0;i<citynum+1;i++)//保存最优解
       {
          groupbest[i]=group[j].city[i];
       }
       groupbestp=group[j].p;
       changebest=0; 
       groupbestfit=group[j].fit;
     }
     else
      changebest=1;//说明新生成的解还不如原来的好，要进行替换
}
/*用最优解替代新种群中的最差的染色体*/
void changebestgroup()
{
  int j,i;
  double fit=group[0].fit;
  j=0;
  if(changebest)
  {
    for(i=1;i<10;i++)
    {
      if(group[i].fit<fit)
      {
        fit=group[i].fit;
        j=i;
      }
    }
    for(i=0;i<citynum+1;i++)
    {
      group[j].city[i]=groupbest[i];
    }
    jisuan();
  }
}
/*初始种群和城市坐标，计算距离*/ 
void chushigroup()
{
  int i,j,t,flag,k;
  double ss;
  cities[0][0]=0;//初始化坐标
  cities[1][0]=0;
  cities[0][1]=2;
  cities[1][1]=0;
  cities[0][2]=4;
  cities[1][2]=0;
  cities[0][3]=6;
  cities[1][3]=0;
  cities[0][4]=6;
  cities[1][4]=2;
  cities[0][5]=6;
  cities[1][5]=4;
  cities[0][6]=6;
  cities[1][6]=6;
  cities[0][7]=5;
  cities[1][7]=5;
  cities[0][8]=4;
  cities[1][8]=4;
  cities[0][9]=2;
  cities[1][9]=2;
memset(groupbest,-1,sizeof(groupbest));
groupbestp=0.0;
groupbestfit=0.0;
changebest=0;
  for(i=0;i<citynum;i++)
    for(j=0;j<=i;j++)
    {
      if(j==i)
          distance[i][j]=0.0;
      else
      {
          distance[i][j]=sqrt(pow(cities[0][i]-cities[0][j],2.0)+pow(cities[1][i]-cities[1][j],2.0));//欧氏距离
          distance[j][i]=distance[i][j];
      }
    }
  printf("最优解的距离是：%f\n",distance[0][1]+distance[1][2]+distance[2][3]+distance[3][4]+distance[4][5]+distance[5][6]+distance[6][7]+distance[7][8]+distance[8][9]+distance[9][0]);
  srand((unsigned)time(NULL));
  ss=0;
  for(k=0;k<10;k++)
  {//一个数量为10的种群，和10 个城市环 
    for(i=0;i<citynum;i++)
    {
        flag=1;
        while(flag)
        {
          t=rand()%citynum;
           for(j=0;j<i;j++)
          {
            if(group[k].city[j]==t)
            {
              break;
            }
          }
            if(j==i)
            {
              group[k].city[i]=t;
              flag=0;
            }
         }
     }
   group[k].city[10]= group[k].city[0];
  }
  //以上产生了10 个种群，分别有不重复的染色体

jisuan();
savebest();
printf("初始种群为：\n");
for(i=0;i<10;i++)
{
  for(j=0;j<citynum+1;j++)
      printf("%d ",group[i].city[j]);
  printf("||适应度：%f，占总群的概率：%f\n",group[i].fit,group[i].p);
}
}
/*选择--复制*/ 
void xuanze()
{
  int i,j,temp[10][11],k;
  double t;
  srand((unsigned)time(NULL));
  for(i=0;i<10;i++) //选10条染色体出来复制,赌轮
  {
    t=rand()%10000*1.0/10000;
    for(j=0;j<10;j++)
    {
        
      if(t<=group[j].jileigailv)
      {
          for(k=0;k<citynum+1;k++)
          {
            temp[i][k]=group[j].city[k];
          }
          break; 
      } 
    }
  }
  //拷贝新种群
  for(i=0;i<10;i++)
    for(j=0;j<citynum+1;j++)
    {
      group[i].city[j]=temp[i][j];
    }
    jisuan();
    savebest();
    changebestgroup();
}
/*交叉*/ 
void jiaocha()
{
  int point1,point2,temp,i,j,k,temp3[2][10],temp2[2][10],num,write;
  srand((unsigned)time(NULL));
  point1=rand()%10;
  point2=rand()%10;
  if(point1>point2)
  {
    temp=point1;
    point1=point2;
    point2=temp;
  }
  //交换,每2条交换
 if(point1!=point2)
 {
  for(j=1;j<10;j=j+2)
  {
       memset(temp3,-1,sizeof(temp3));
       memset(temp2,-1,sizeof(temp2));
       k=0;
    for(i=point1;i<=point2;i++)
    {
     temp2[0][k]=group[j].city[i];
     temp2[1][k]=group[j-1].city[i];
     temp3[0][temp2[0][k]]=1;//标记数字已经存在了
     temp3[1][temp2[1][k]]=1;
     k++;
     group[j].city[i]=-1;
     group[j-1].city[i]=-1;
    }
    num=point2-point1+1;//交换的位数 
     //消重
    for(k=0;k<point1;k++)
      {
        if(temp3[0][group[j-1].city[k]]==1)
        {
           group[j-1].city[k]=-1;
        }
        else
           temp3[0][group[j-1].city[k]]=1;
      } 
      for(k=point2+1;k<citynum;k++)
      {
        if(temp3[0][group[j-1].city[k]]==1)
        {
           group[j-1].city[k]=-1;
        }
        else
           temp3[0][group[j-1].city[k]]=1;
      }
      for(k=0;k<point1;k++)
      {
        if(temp3[1][group[j].city[k]]==1)
        {
           group[j].city[k]=-1;
        }
        else
           temp3[1][group[j].city[k]]=1;
      } 
      for(k=point2+1;k<citynum;k++)
      {
        if(temp3[1][group[j].city[k]]==1)
        {
           group[j].city[k]=-1;
        }
        else
           temp3[1][group[j].city[k]]=1;
      }
      write=0; 
      for(i=0;i<10;i++)
      {
      while(write<10&&group[j-1].city[write]==-1)
      {
        write++;
      }
      if(write<10)
      {
        temp=group[j-1].city[i];
        group[j-1].city[i]=group[j-1].city[write];
        group[j-1].city[write]=temp;
        write++;
      }
      else
      {
          write=0;
      for(k=i;k<10;k++)
      {
          group[j-1].city[k]=temp2[0][write++];
          if(write==num)
          break;
       }
      break;
      }
    } 
     write=0; 
     for(i=0;i<10;i++)
     {
      while(write<10&&group[j].city[write]==-1)
      {
        write++;
      }
      if(write<10)
      {
        temp=group[j].city[i];
        group[j].city[i]=group[j].city[write];
        group[j].city[write]=temp;
        write++;
      }
      else
      {
          write=0;
        for(k=i;k<10;k++)
        {
          group[j].city[k]=temp2[1][write++];
          if(write==num)
              break;
         }
        break;
      }
    } 
    k=0;
    for(i=0;i<citynum;i++)
    {
       if(group[j-1].city[i]==-1)
       {
         while(temp3[0][k]==1&&k<10)
         {
           k++;
         }
         group[j-1].city[i]=k++;
       }
    }
    k=0;
    for(i=0;i<citynum;i++)
    {
       if(group[j].city[i]==-1)
       {
         while(temp3[1][k]==1&&k<10)
         {
           k++;
         }
         group[j].city[i]=k++;
       }
    }
   group[j].city[10]=group[j].city[0];
   group[j-1].city[10]=group[j-1].city[0];
  }//end of j
 jisuan();
 savebest();
 changebestgroup();
 } //end of if(!)
}
/*变异*/ 
void bianyi()
{
   int t1,t2,temp,t,s=3;
   srand((unsigned)time(NULL));
   t=rand()%10;
   if(t>3)//变异率为3/5 
   {
       //挑1个不同的变异，只交换一位
    t1=rand()%10;//种群 
    t2=rand()%10;//变换位
    temp=group[t1].city[t2];
    group[t1].city[t2]=group[t1].city[9-t2];
    group[t1].city[9-t2]=temp;
    group[t1].city[10]=group[t1].city[0];   
   }
   jisuan();
   savebest();
   changebestgroup();
}
/*主函数*/
int  main()//最优解20.485281
{
  int j,flag,tuichu=1;
  double distancenum;
  while(tuichu)
  {
    distancenum=0.0;
    flag=1;
    chushigroup();
    while(flag)
    {
    printf("请输入种群繁衍代数（1000以内）：");
    scanf("%d",&die);
    if(die<=1000)
    {
    flag=0;
    }
  }
  while(die--)
  {
     xuanze();
     jiaocha();
     bianyi();
  }
 printf("最优种群是：\n");
      for(j=0;j<citynum+1;j++)
      {
        printf("%d ",groupbest[j]);
        if(j<citynum)
        {
        distancenum+=distance[groupbest[j]][groupbest[j+1]];
        }
      }
      printf("距离为:%f,适应度为：%f,代数：%d\n\n",distancenum,groupbestfit,dai);
      printf("继续产生新种群请按输入1，退出请输入0：");
      scanf("%d",&tuichu); 
      printf("\n");
    }
      return 0;
}
