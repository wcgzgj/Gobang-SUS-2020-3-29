#include<iostream>
#include<vector>

#define MAX_VALUE 67108864

#define LAY 901
#define STAND 902
#define MAIN 903
#define VICE 904

#define HH 801
#define HL 802
#define LH 803
#define LL 804

using namespace std;

struct chess
{
	int x;
	int y;
	int value;
	chess()
	{}
	chess(int xL,int yL,int valueL)
	{
		x=xL;
		y=yL;
		value=valueL;
	}	
};

struct location
{
	int x;
	int y;
	location()
	{}

	location(int x,int y)
	{
		this->x=x;
		this->y=y;
	}
};

class Array
{
private:
	int **a;
	int len;
	int side;
public:
	Array()
	{
		len=10;//
		side=1;//
		a=new int *[len+1];
		for(int i=0;i<len+1;i++)
		{
			a[i]=new int[len+1];
		}

		for(int i=0;i<len+1;i++)
		{
			for(int j=0;j<len+1;j++)
			{
				a[i][j]=0;
			}
		}
	}

	void cinMap()
	{
		for(int i=1;i<=len;i++)
		{
			for(int j=1;j<=len;j++)
			{
				cin>>a[i][j];
			}
		}
	}

	void outputNum(int x,int y)
	{
		cout<<x*11+y<<endl;
	}

	void outputNumMap()
	{
		for(int i=1;i<=len;i++)
		{
			for(int j=1;j<=len;j++)
			{
				cout<<a[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}

	void outputMap()
	{
		side=this->side;
		//cout<<"  ";
		//for(int i=1;i<=len;i++) cout<<i;
		cout<<endl;
		for(int i=1;i<=len;i++)
		{
			//cout<<i;
			for(int j=1;j<=len;j++)
			{
				if(getValue(i,j)==0) cout<<"+";
				else if(getValue(i,j)==1) cout<<"*";
				else cout<<"o";
			}
			cout<<endl;
		}
		
	}

	void AI()
	{
		chess ce=getBestChess();
		while(getValue(ce.x,ce.y)!=0)
		{
			ce.x-=1;
			ce.y-=1;
		}
		a[ce.x][ce.y]=side;
		cout<<ce.x<<" "<<ce.y<<"fuck"<<endl;
	}

	int win()
	{
		int enemy;
		if(side==1) enemy=2;
		if(side==2) enemy=1;
		int side=this->side;
		for(int i=1;i<=len;i++)
		{
			for(int j=1;j<=len;j++)
			{
				if(linkFive(i,j,side)!=0  && getValue(i,j)==side) return 1;
				if(linkFive(i,j,enemy)!=0  && getValue(i,j)==enemy) return 1;
			}
		}
		return 0;
	}

	void putChess(int x,int y)
	{
		if(side==1) a[x][y]=2;
		if(side==2) a[x][y]=1;
	}

private://赋分函数
	

	chess getBestChess()
	{
		int count=0;
		vector<chess> lc;
		for(int i=1;i<=len;i++)
		{
			for(int j=1;j<=len;j++)
			{
				if(getValue(i,j)==0)
				{
					chess cs=getChess(i,j);
					lc.push_back(cs);
					count++;
				}
			}
		}
		for(int i=0;i<count;i++)
			cout<<lc[i].value<<" ";
		cout<<endl;
		int tmp=lc[0].value;
		int flag=0;
		for(int i=0;i<count;i++)
		{
			if(tmp!=lc[i].value) flag=1;
		}
		if(flag==0) return chess(len/2,len/2,0);

		int max=lc[0].value;//找权值最大的子
		flag=0;
		for(int i=0;i<count;i++)
		{
			if(lc[i].value>max) flag=i;
		}
		return lc[flag];
	}

	chess getChess(int x,int y)
	{
		chess ce;
		int valueAtk=giveValue(x,y,1);
		int valueDef=giveValue(x,y,2);
		ce.value=valueAtk>=valueDef?valueAtk:valueDef;
		ce.x=x;
		ce.y=y;
		return ce;
	}

	int giveValue(int x,int y,int side)
	{
		if(getValue(x,y)==0) return 0;
		int enemy;
		if(side==1) enemy=2;
		else enemy=1;
		if(getValue(x,y)==side) return atkValue(x,y,side);
		if(getValue(x,y)==enemy) return defValue(x,y,enemy);
	}

	int atkValue(int x,int y,int side)
	{
		int value=0;
		if(linkFive(x,y,side)!=0) return MAX_VALUE;
		if(liveFour(x,y,side)!=0) return MAX_VALUE/2/2/2;

		if(liveThree(x,y,side)>1) return MAX_VALUE/2/2/2/2;
		if(liveFour(x,y,side)>1) return MAX_VALUE/2/2/2/2;
		if(rushFour(x,y,side)>1) return MAX_VALUE/2/2/2/2;
		if(rushFour(x,y,side)!=0 && liveFour(x,y,side)!=0) return MAX_VALUE/2/2/2/2;

		value+=rushFour(x,y,side)*MAX_VALUE/2/2/2/2/2/2;

		value+=liveThree(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2/2;

		value+=sleepThree(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2/2/2/2;

		value+=liveTwo(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2/2/2/2/2/2;

		value+=sleepTwo(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2/2/2/2/2/2/2;

		return value;
	}

	int defValue(int x,int y,int side)
	{
		int value=0;
		if(linkFive(x,y,side)!=0) return MAX_VALUE/2;
		if(liveFour(x,y,side)!=0) return MAX_VALUE/2/2;

		if(liveThree(x,y,side)>1) return MAX_VALUE/2/2/2/2/2;
		if(liveFour(x,y,side)>1) return MAX_VALUE/2/2/2/2/2;
		if(rushFour(x,y,side)>1) return MAX_VALUE/2/2/2/2/2;
		if(rushFour(x,y,side)!=0 && liveFour(x,y,side)!=0) return MAX_VALUE/2/2/2/2/2;

		value+=rushFour(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2;

		value+=liveThree(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2/2/2;

		value+=sleepThree(x,y,side)*MAX_VALUE/2/2/2/2/2/2/2/2/2/2/2;

		return value;

	}

private://工具函数
	void outputBlank(int n)
	{
		for(int i=0;i<n;i++)
		{
			cout<<" ";
		}
	}

	void outputDot(int n)
	{
		for(int i=0;i<n;i++)
		{
			cout<<"·";
		}
	}

	void outputEndl(int n)
	{
		for(int i=0;i<n;i++) cout<<endl;
	}
	
	void outputTwoStandLine(int n)
	{

	}

private://连五到眠二所有赋分函数

	int linkFive(int x,int y,int side)//连5
	{
		if(getMaxNum(x,y,side)>=5) return 1;
		return 0;
	}

	int liveFour(int x,int y,int side)//活4
	{
		int count=0;
		if(layNum(x,y,side)==4 && getHeadTail(x,y,side,LAY)==LL) count++;
		if(standNum(x,y,side)==4 && getHeadTail(x,y,side,STAND)==LL) count++;
		if(mainNum(x,y,side)==4 && getHeadTail(x,y,side,MAIN)==LL) count++;
		if(viceNum(x,y,side)==4 && getHeadTail(x,y,side,VICE)==LL) count++;
		return count;
	}

	int rushFour(int x,int y,int side)
	{
		int count=0;
		//========================连叉冲4
		count+=linkChaRushFour(x,y,side,LAY);
		count+=linkChaRushFour(x,y,side,STAND);
		count+=linkChaRushFour(x,y,side,MAIN);
		count+=linkChaRushFour(x,y,side,VICE);
		//========================3-1冲4
		count+=threeOneRushFour(x,y,side,LAY);
		count+=threeOneRushFour(x,y,side,STAND);
		count+=threeOneRushFour(x,y,side,MAIN);
		count+=threeOneRushFour(x,y,side,VICE);
		//========================2-2冲4
		count+=twoTwoRushFour(x,y,side,LAY);
		count+=twoTwoRushFour(x,y,side,STAND);
		count+=twoTwoRushFour(x,y,side,MAIN);
		count+=twoTwoRushFour(x,y,side,VICE);

		return count;
	}

	int liveThree(int x,int y,int side)
	{
		int count=0;
		//==========================连活3
		count+=linkLiveThree(x,y,side,LAY);
		count+=linkLiveThree(x,y,side,STAND);
		count+=linkLiveThree(x,y,side,MAIN);
		count+=linkLiveThree(x,y,side,VICE);

		//==========================跳活3
		count+=jumpLiveThree(x,y,side,LAY);
		count+=jumpLiveThree(x,y,side,STAND);
		count+=jumpLiveThree(x,y,side,MAIN);
		count+=jumpLiveThree(x,y,side,VICE);

		return count;
	}

	int sleepThree(int x,int y,int side)
	{
		int count=0;
		//========================连叉眠3
		count+=linkChaSleepThree(x,y,side,LAY);
		count+=linkChaSleepThree(x,y,side,STAND);
		count+=linkChaSleepThree(x,y,side,MAIN);
		count+=linkChaSleepThree(x,y,side,VICE);
		//========================1-2叉眠3
		count+=oneTwoChaSleepThree(x,y,side,LAY);
		count+=oneTwoChaSleepThree(x,y,side,STAND);
		count+=oneTwoChaSleepThree(x,y,side,MAIN);
		count+=oneTwoChaSleepThree(x,y,side,VICE);
		
		//========================2-1叉眠3
		count+=twoOneChaSleepThree(x,y,side,LAY);
		count+=twoOneChaSleepThree(x,y,side,STAND);
		count+=twoOneChaSleepThree(x,y,side,MAIN);
		count+=twoOneChaSleepThree(x,y,side,VICE);
		
		//========================2-1眠3
		count+=twoOneSleepThree(x,y,side,LAY);
		count+=twoOneSleepThree(x,y,side,STAND);
		count+=twoOneSleepThree(x,y,side,MAIN);
		count+=twoOneSleepThree(x,y,side,VICE);
		//========================1-1-1眠3
		count+=oneOneOneSleepThree(x,y,side,LAY);
		count+=oneOneOneSleepThree(x,y,side,STAND);
		count+=oneOneOneSleepThree(x,y,side,MAIN);
		count+=oneOneOneSleepThree(x,y,side,VICE);
		
		//========================叉连叉眠3
		count+=chaLinkChaSleepThree(x,y,side,LAY);
		count+=chaLinkChaSleepThree(x,y,side,STAND);
		count+=chaLinkChaSleepThree(x,y,side,MAIN);
		count+=chaLinkChaSleepThree(x,y,side,VICE);

		return count;
	}

	int liveTwo(int x,int y,int side)
	{
		int count=0;
		//=====================连活2
		count+=linkLiveTwo(x,y,side,LAY);
		count+=linkLiveTwo(x,y,side,STAND);
		count+=linkLiveTwo(x,y,side,MAIN);
		count+=linkLiveTwo(x,y,side,VICE);
		//=====================跳活2
		count+=jumpLiveTwo(x,y,side,LAY);
		count+=jumpLiveTwo(x,y,side,STAND);
		count+=jumpLiveTwo(x,y,side,MAIN);
		count+=jumpLiveTwo(x,y,side,VICE);
		//=====================大跳活2
		count+=bigJumpLiveTwo(x,y,side,LAY);
		count+=bigJumpLiveTwo(x,y,side,STAND);
		count+=bigJumpLiveTwo(x,y,side,MAIN);
		count+=bigJumpLiveTwo(x,y,side,VICE);

		return count;
	}

	int sleepTwo(int x,int y,int side)
	{
		int count=0;
		//=======================连叉眠2
		count+=linkChaSleepTwo(x,y,side,LAY);
		count+=linkChaSleepTwo(x,y,side,STAND);
		count+=linkChaSleepTwo(x,y,side,MAIN);
		count+=linkChaSleepTwo(x,y,side,VICE);

		//=======================跳叉眠2
		count+=jumpChaSleepTwo(x,y,side,LAY);
		count+=jumpChaSleepTwo(x,y,side,STAND);
		count+=jumpChaSleepTwo(x,y,side,MAIN);
		count+=jumpChaSleepTwo(x,y,side,VICE);
		//=======================大跳叉眠2
		count+=bigJumpChaSleepTwo(x,y,side,LAY);
		count+=bigJumpChaSleepTwo(x,y,side,STAND);
		count+=bigJumpChaSleepTwo(x,y,side,MAIN);
		count+=bigJumpChaSleepTwo(x,y,side,VICE);
		//=======================超级跳眠2
		count+=superJumpSleepTwo(x,y,side,LAY);
		count+=superJumpSleepTwo(x,y,side,STAND);
		count+=superJumpSleepTwo(x,y,side,MAIN);
		count+=superJumpSleepTwo(x,y,side,VICE);
		
		return count;
	}

private://各种阵型的分阵型

	int linkChaRushFour(int x,int y,int side,int dir)
	{
		if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) return 1;
		return 0;
	}

	int threeOneRushFour(int x,int y,int side,int dir)
	{
		int count=0;
		if(getNum(x,y,side,dir)==3 || getNum(x,y,side,dir)==1)
		{
			a[x][y]=side;
			location head=getHeadLocation(x,y,side,dir);
			location tail=getTailLocation(x,y,side,dir);
			if(getHeadTail(x,y,side,dir)==HL)
			{
				giveNextSide(tail.x,tail.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetNextSide(tail.x,tail.y,side,dir);
			}
			if(getHeadTail(x,y,side,dir)==LH)
			{
				givePrevSide(head.x,head.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetPrevSide(head.x,head.y,side,dir);
			}
			if(getHeadTail(x,y,side,dir)==LL)
			{
				givePrevSide(head.x,head.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetPrevSide(head.x,head.y,side,dir);

				giveNextSide(tail.x,tail.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetNextSide(tail.x,tail.y,side,dir);
			}
			a[x][y]=0;
		}
		return count;
	}

	int twoTwoRushFour(int x,int y,int side,int dir)//2-2冲4
	{
		int count=0;
		if(getNum(x,y,side,dir)==2)
		{
			a[x][y]=side;
			location head=getHeadLocation(x,y,side,dir);
			location tail=getTailLocation(x,y,side,dir);

			if(getHeadTail(x,y,side,dir)==LL)
			{
				givePrevSide(head.x,head.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetPrevSide(head.x,head.y,side,dir);

				giveNextSide(tail.x,tail.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetNextSide(tail.x,tail.y,side,dir);
			}
			if(getHeadTail(x,y,side,dir)==LH)
			{
				givePrevSide(head.x,head.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetPrevSide(head.x,head.y,side,dir);
			}
			if(getHeadTail(x,y,side,dir)==HL)
			{
				giveNextSide(tail.x,tail.y,side,dir);
				if(getNum(x,y,side,dir)>=5) count++;
				resetNextSide(tail.x,tail.y,side,dir);
			}
			a[x][y]=0;
		}
		return count;
	}

	int linkLiveThree(int x,int y,int side,int dir)
	{
		if(getNum(x,y,side,dir)==3 && getHeadTail(x,y,side,dir)==LL) return 1;
		return 0;
	}

	int jumpLiveThree(int x,int y,int side,int dir)
	{
		a[x][y]=side;
		int count=0;
		if(getNum(x,y,side,dir)==2 && getHeadTail(x,y,side,dir)==LL)
		{	
			location head=getHeadLocation(x,y,side,dir);
			location tail=getTailLocation(x,y,side,dir);

			givePrevSide(head.x,head.y,side,dir);
			if(getNum(x,y,side,dir)==4) count++;
			resetPrevSide(head.x,head.y,side,dir);

			giveNextSide(tail.x,tail.y,side,dir);
			if(getNum(x,y,side,dir)==4) count++;
			resetNextSide(tail.x,tail.y,side,dir);
		}
		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)==LL)
		{
			location head=getHeadLocation(x,y,side,dir);
			location tail=getTailLocation(x,y,side,dir);

			givePrevSide(head.x,head.y,side,dir);
			if(getNum(x,y,side,dir)==4) count++;
			resetPrevSide(head.x,head.y,side,dir);

			giveNextSide(tail.x,tail.y,side,dir);
			if(getNum(x,y,side,dir)==4) count++;
			resetNextSide(tail.x,tail.y,side,dir);
		}
		a[x][y]=0;

		return count;
	}

	int linkChaSleepThree(int x,int y,int side,int dir)
	{
		int count=0;
		a[x][y]=side;
		if(getNum(x,y,side,dir)==3 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL))
		{
			location head=getHeadLocation(x,y,side,dir);
			location tail=getTailLocation(x,y,side,dir);

			givePrevSide(head.x,head.y,side,dir);
			if(linkChaRushFour(x,y,side,dir)!=0)  count++;
			resetPrevSide(head.x,head.y,side,dir);

			giveNextSide(tail.x,tail.y,side,dir);
			if(linkChaRushFour(x,y,side,dir)!=0)  count++;
			resetNextSide(tail.x,tail.y,side,dir);
		}
		a[x][y]=0;
		return count;
	}

	int oneTwoChaSleepThree(int x,int y,int side,int dir)
	{
		int count=0;
		location head=getHeadLocation(x,y,side,dir);
		location tail=getTailLocation(x,y,side,dir);
		a[x][y]=side;
		
		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)==LL)
		{
			
			giveNextSide(tail.x,tail.y,side,dir);
			if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
			resetNextSide(tail.x,tail.y,side,dir);

			givePrevSide(head.x,head.y,side,dir);
			if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
			resetPrevSide(head.x,head.y,side,dir);
		}
		if(getNum(x,y,side,dir)==2 && (getHeadTail(x,y,side,dir)==HL ||getHeadTail(x,y,side,dir)==LH))
		{
			if(getHeadTail(x,y,side,dir)==LH)
			{
				givePrevSide(head.x,head.y,side,dir);
				if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
				resetPrevSide(head.x,head.y,side,dir);
			}
			if(getHeadTail(x,y,side,dir)==HL)
			{
				giveNextSide(tail.x,tail.y,side,dir);
				if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
				resetNextSide(tail.x,tail.y,side,dir);
			}

		}
		a[x][y]=0;
		return count;
	}

	int twoOneChaSleepThree(int x,int y,int side,int dir)
	{
		int count=0;
		location head=getHeadLocation(x,y,side,dir);
		location tail=getTailLocation(x,y,side,dir);
		a[x][y]=side;
		if(getNum(x,y,side,dir)==2 && getHeadTail(x,y,side,dir)==LL)
		{
			
			givePrevSide(head.x,head.y,side,dir);
			if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
			resetPrevSide(head.x,head.y,side,dir);

			giveNextSide(tail.x,tail.y,side,dir);
			if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
			resetNextSide(tail.x,tail.y,side,dir);
		}
		if(getNum(x,y,side,dir)==1 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL))
		{
			if(getHeadTail(x,y,side,dir)==LH)
			{
				givePrevSide(head.x,head.y,side,dir);
				if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
				resetPrevSide(head.x,head.y,side,dir);
			}
			if(getHeadTail(x,y,side,dir)==HL)
			{
				giveNextSide(tail.x,tail.y,side,dir);
				if(getNum(x,y,side,dir)==4 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
				resetNextSide(tail.x,tail.y,side,dir);
			}
		}
		a[x][y]=0;
		return count;
	}

	int twoOneSleepThree(int x,int y,int side,int dir)//<=======改，不然会误伤
	{
		int count=0;
		location head=getHeadLocation(x,y,side,dir);
		location tail=getTailLocation(x,y,side,dir);
		a[x][y]=side;

		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)!=HH)
		{
			if(getHeadTail(x,y,side,dir)==HL)
			{
				location k1(getNextLocation(x,y,dir).x,getNextLocation(x,y,dir).y);
				location k2(getNextLocation(k1.x,k1.y,dir).x,getNextLocation(k1.x,k1.y,dir).y);
				location k3(getNextLocation(k2.x,k2.y,dir).x,getNextLocation(k2.x,k2.y,dir).y);
				location k4(getNextLocation(k3.x,k3.y,dir).x,getNextLocation(k3.x,k3.y,dir).y);
				if(inLimits(k1) && inLimits(k2) && inLimits(k2) && inLimits(k4))
				{
					if(getValue(k1)==0 && getValue(k2)==0 && getValue(k3)==side && getValue(k4)==side) count++;
				}
			}
			if(getHeadTail(x,y,side,dir)==LH)
			{
				location k1(getPrevLocation(x,y,dir).x,getPrevLocation(x,y,dir).y);
				location k2(getPrevLocation(k1.x,k1.y,dir).x,getPrevLocation(k1.x,k1.y,dir).y);
				location k3(getPrevLocation(k2.x,k2.y,dir).x,getPrevLocation(k2.x,k2.y,dir).y);
				location k4(getPrevLocation(k3.x,k3.y,dir).x,getPrevLocation(k3.x,k3.y,dir).y);
				if(inLimits(k1) && inLimits(k2) && inLimits(k2) && inLimits(k4))
				{
					if(getValue(k1)==0 && getValue(k2)==0 && getValue(k3)==side && getValue(k4)==side) count++;
				}
			}
			if(getHeadTail(x,y,side,dir)==LL)
			{
				location k1(getNextLocation(x,y,dir).x,getNextLocation(x,y,dir).y);
				location k2(getNextLocation(k1.x,k1.y,dir).x,getNextLocation(k1.x,k1.y,dir).y);
				location k3(getNextLocation(k2.x,k2.y,dir).x,getNextLocation(k2.x,k2.y,dir).y);
				location k4(getNextLocation(k3.x,k3.y,dir).x,getNextLocation(k3.x,k3.y,dir).y);
				if(inLimits(k1) && inLimits(k2) && inLimits(k2) && inLimits(k4))
				{
					if(getValue(k1)==0 && getValue(k2)==0 && getValue(k3)==side && getValue(k4)==side) count++;
				}

				location p1(getPrevLocation(x,y,dir).x,getPrevLocation(x,y,dir).y);
				location p2(getPrevLocation(p1.x,p1.y,dir).x,getPrevLocation(p1.x,p1.y,dir).y);
				location p3(getPrevLocation(p2.x,p2.y,dir).x,getPrevLocation(p2.x,p2.y,dir).y);
				location p4(getPrevLocation(p3.x,p3.y,dir).x,getPrevLocation(p3.x,p3.y,dir).y);
				if(inLimits(p1) && inLimits(p2) && inLimits(p2) && inLimits(p4))
				{
					if(getValue(p1)==0 && getValue(p2)==0 && getValue(p3)==side && getValue(p4)==side) count++;
				}		
			}
		}

		if(getNum(x,y,side,dir)==2 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL))
		{//后来我改写了giveNextSide()和givePrevSide()函数，让它们判断如果不是0，就不再赋side  
		 //ps:因为getHeadTail()中的H只有2的情况，所以不用担心reset的时候会把原本的1给变成0
			giveNextSide(tail.x,tail.y,side,dir);
			if(threeOneRushFour(x,y,side,dir)!=0) count++;
			resetNextSide(tail.x,tail.y,side,dir);

			givePrevSide(head.x,head.y,side,dir);
			if(threeOneRushFour(x,y,side,dir)!=0) count++;
			resetPrevSide(head.x,head.y,side,dir);	
		}

		a[x][y]=0;
		return count;
	}

	int oneOneOneSleepThree(int x,int y,int side,int dir)
	{
		int count=0;
		location head=getHeadLocation(x,y,side,dir);
		location tail=getTailLocation(x,y,side,dir);
		a[x][y]=side;
		
		if(getNum(x,y,side,dir)==1 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL))
		{
			giveNextSide(tail.x,tail.y,side,dir);
			if(threeOneRushFour(x,y,side,dir)!=0) count++;
			resetNextSide(tail.x,tail.y,side,dir);


			givePrevSide(head.x,head.y,side,dir);
			if(threeOneRushFour(x,y,side,dir)!=0) count++;
			resetPrevSide(head.x,head.y,side,dir);
			
		}
		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)==LL)
		{//随便左边右边其实都可以
			giveNextSide(tail.x,tail.y,side,dir);
			if(threeOneRushFour(x,y,side,dir)!=0) count++;
			resetNextSide(tail.x,tail.y,side,dir);
		}

		a[x][y]=0;
		return count;
	}

	int chaLinkChaSleepThree(int x,int y,int side,int dir)
	{
		int count=0;
		location head=getHeadLocation(x,y,side,dir);
		location tail=getTailLocation(x,y,side,dir);
		if(getNum(x,y,side,dir)==3 && getHeadTail(x,y,side,dir)==LL)
		{
			giveNextSide(tail.x,tail.y,side,dir);
			givePrevSide(head.x,head.y,side,dir);
			if(getNum(x,y,side,dir)==5 && getHeadTail(x,y,side,dir)==HH) count++;
			resetNextSide(tail.x,tail.y,side,dir);
			resetPrevSide(head.x,head.y,side,dir);
		}
		return count;	
	}

	int linkLiveTwo(int x,int y,int side,int dir)
	{
		int count=0;
		if(getNum(x,y,side,dir)==2 && getHeadTail(x,y,side,dir)==LL)
		{
			location head=getHeadLocation(x,y,side,dir);
			location tail=getTailLocation(x,y,side,dir);

			location k1(getNextLocation(tail.x,tail.y,dir).x,getNextLocation(tail.x,tail.y,dir).y);
			location k2(getNextLocation(k1.x,k1.y,dir).x,getNextLocation(k1.x,k1.y,dir).y);
			location k3(getPrevLocation(head.x,head.y,dir).x,getPrevLocation(head.x,head.y,dir).y);
			location k4(getPrevLocation(k3.x,k3.y,dir).x,getPrevLocation(k3.x,k3.y,dir).y);
			if(inLimits(k1,k2,k3,k4) && getValue(k1)==0 && getValue(k2)==0 && getValue(k3)==0 && getValue(k4)==0)  count++;
		}
		return count;
	}

	int jumpLiveTwo(int x,int y,int side,int dir)
	{
		int count=0;
		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)==LL)
		{
			location k1=getPrevLocation(x,y,dir);
			location k2=getNextLocation(x,y,dir);

			a[k1.x][k1.y]=side;
			a[k2.x][k2.y]=side;
			if(getNum(x,y,side,dir)>=4 && getHeadTail(x,y,side,dir)!=HH) count++;
			a[k1.x][k1.y]=0;
			a[k2.x][k2.y]=0;
		}
		return count;
	}

	int bigJumpLiveTwo(int x,int y,int side,int dir)
	{
		int count=0;
		if(getHeadTail(x,y,side,dir)!=HH)
		{
			location k1=getPrevLocation(x,y,dir);
			location k2=getPrevLocation(k1.x,k1.y,dir);
			location k3=getPrevLocation(k2.x,k2.y,dir);

			location p1=getNextLocation(x,y,dir);
			location p2=getNextLocation(p1.x,p1.y,dir);
			location p3=getNextLocation(p2.x,p2.y,dir);

			if(inLimits(k1)&&inLimits(k2)&&inLimits(k3))
			{
				if(getValue(k1)==0 && getValue(k2)==0 && getValue(k3)==side) count++;
			}

			if(inLimits(p1)&&inLimits(p2)&&inLimits(p3))
			{
				if(getValue(p1)==0 && getValue(p2)==0 && getValue(p3)==side) count++;
			}
		}
		return count;
	}

	int linkChaSleepTwo(int x,int y,int side,int dir)
	{
		int count=0;
		location head=getHeadLocation(x,y,side,dir);
		location tail=getTailLocation(x,y,side,dir);
		if(getNum(x,y,side,dir)==2 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL))
		{
			if(getHeadTail(x,y,side,dir)==LH)
			{
				location k1=getPrevLocation(head.x,head.y,dir);
				location k2=getPrevLocation(k1.x,k1.y,dir);
				location k3=getPrevLocation(k2.x,k2.y,dir);
				if(inLimits(k1) && inLimits(k3) && inLimits(k2) && (getValue(k1)==getValue(k2)==getValue(k3)==0)) count++;
			}
			if(getHeadTail(x,y,side,dir)==HL)
			{
				location k1=getNextLocation(tail.x,tail.y,dir);
				location k2=getNextLocation(k1.x,k1.y,dir);
				location k3=getNextLocation(k2.x,k2.y,dir);
				if(inLimits(k1) && inLimits(k3) && inLimits(k2) && (getValue(k1)==getValue(k2)==getValue(k3)==0)) count++;
			}
		}
		//cout<<"1+"<<dir<<"+"<<count<<endl;
		return count;
	}

	int jumpChaSleepTwo(int x,int y,int side,int dir)
	{
		int count=0;
		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)==LL)
		{
			giveNextSide(x,y,side,dir);
			givePrevSide(x,y,side,dir);
			if(getNum(x,y,side,dir)==4 &&(getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL)) count++;
			resetNextSide(x,y,side,dir);
			resetPrevSide(x,y,side,dir);
		}
		if(getNum(x,y,side,dir)==1 && (getHeadTail(x,y,side,dir)==LH || getHeadTail(x,y,side,dir)==HL))
		{
			if(getHeadTail(x,y,side,dir)==LH)
			{
				givePrevSide(x,y,side,dir);
				if(getNum(x,y,side,dir)==3 && getHeadTail(x,y,side,dir)==LH)
				{
					location head=getHeadLocation(x,y,side,dir);
					location k1=getPrevLocation(head.x,head.y,dir);
					location k2=getPrevLocation(k1.x,k1.y,dir);
					if(inLimits(k1) && inLimits(k2) &&(getValue(k1)==getValue(k2)==0)) count++;
				}
				resetPrevSide(x,y,side,dir);
			}

			if(getHeadTail(x,y,side,dir)==HL)
			{
				giveNextSide(x,y,side,dir);
				if(getNum(x,y,side,dir)==3 && getHeadTail(x,y,side,dir)==HL)
				{
					location tail=getTailLocation(x,y,side,dir);
					location k1=getNextLocation(tail.x,tail.y,dir);
					location k2=getNextLocation(k1.x,k1.y,dir);
					if(inLimits(k1) && inLimits(k2) &&(getValue(k1)==getValue(k2)==0)) count++;
				}
				resetNextSide(x,y,side,dir);
			}
		}
		//cout<<"2+"<<dir<<"+"<<count<<endl;
		return count;
	}

	int bigJumpChaSleepTwo(int x,int y,int side,int dir)//<+=====================fuckerrrrrrrr
	{
		int count=0;
		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)==LL)
		{
			location k1=getNextLocation(x,y,dir);
			location k2=getNextLocation(k1.x,k1.y,dir);
			location k3=getNextLocation(k2.x,k2.y,dir);
			location k4=getNextLocation(k3.x,k3.y,dir);
			location k5=getPrevLocation(x,y,dir);

			location p1=getPrevLocation(x,y,dir);
			location p2=getPrevLocation(p1.x,p1.y,dir);
			location p3=getPrevLocation(p2.x,p2.y,dir);
			location p4=getPrevLocation(p3.x,p3.y,dir);
			location p5=getNextLocation(x,y,dir);

			
			//cout<<dir<<"+"<<getValue(k1)<<getValue(k2)<<getValue(k3)<<getValue(k4)<<getValue(k5)<<endl;
			//cout<<dir<<"+"<<getValue(p1)<<getValue(p2)<<getValue(p3)<<getValue(p4)<<getValue(p5)<<endl;
			if(inLimits(k1,k2,k3,k4)&&inLimits(k5) && (getValue(k1)==getValue(k2)==getValue(k5)==0) && getValue(k3)==side &&(getValue(k4)!=0 && getValue(k4)!=side))
				count++;
			if(inLimits(p1,p2,p3,p4)&&inLimits(p5) && (getValue(p1)==getValue(p2)==getValue(p5)==0) && getValue(p3)==side &&(getValue(p4)!=0 && getValue(p4)!=side))
				count++;
		}

		if(getNum(x,y,side,dir)==1 && getHeadTail(x,y,side,dir)!=LL)
		{
			if(getHeadTail(x,y,side,dir)==LH)
			{
				location k1=getPrevLocation(x,y,dir);
				location k2=getPrevLocation(k1.x,k1.y,dir);
				location k3=getPrevLocation(k2.x,k2.y,dir);
				location k4=getPrevLocation(k3.x,k3.y,dir);
				if(inLimits(k1)&& inLimits(k2) && inLimits(k3)&& inLimits(k4) && 
				(getValue(k1)==getValue(k2)==getValue(k4)==0) && getValue(k3)==side)
					count++;
			}

			if(getHeadTail(x,y,side,dir)==HL)
			{
				location k1=getNextLocation(x,y,dir);
				location k2=getNextLocation(k1.x,k1.y,dir);
				location k3=getNextLocation(k2.x,k2.y,dir);
				location k4=getNextLocation(k3.x,k3.y,dir);
				if(inLimits(k1)&& inLimits(k2) && inLimits(k3)&& inLimits(k4) && 
				(getValue(k1)==getValue(k2)==getValue(k4)==0) && getValue(k3)==side)
					count++;
			}
		}
		//cout<<"3+"<<dir<<"+"<<count<<endl;
		return count;
	}

	int superJumpSleepTwo(int x,int y,int side,int dir)
	{
		int count=0;
		location k1=getNextLocation(x,y,dir);
		location k2=getNextLocation(k1.x,k1.y,dir);
		location k3=getNextLocation(k2.x,k2.y,dir);
		location k4=getNextLocation(k3.x,k3.y,dir);

		location p1=getPrevLocation(x,y,dir);
		location p2=getPrevLocation(p1.x,p1.y,dir);
		location p3=getPrevLocation(p2.x,p2.y,dir);
		location p4=getPrevLocation(p3.x,p3.y,dir);

		if(inLimits(k1,k2,k3,k4) && (getValue(k1)==getValue(k2)==getValue(k3)==0) && getValue(k4)==side) count++;
		if(inLimits(p1,p2,p3,p4) && (getValue(p1)==getValue(p2)==getValue(p3)==0) && getValue(p4)==side) count++;
		//cout<<"4+"<<dir<<"+"<<count<<endl;
		return count;
	}

//==========================工具函数
private:

	void giveNextSide(int x,int y,int side,int dir)
	{
		switch(dir)
		{
		case LAY:
			if(y+1>len || a[x][y+1]!=0 ) return;
			a[x][y+1]=side;
			break;
		case STAND:
			if(x+1>len || a[x+1][y]!=0 ) return;
			a[x+1][y]=side;
			break;
		case MAIN:
			if(x+1>len || y+1>len || a[x+1][y+1]!=0 ) return;
			a[x+1][y+1]=side;
			break;
		case VICE:
			if( x-1<1 || y+1>len || a[x-1][y+1]!=0 ) return;
			a[x-1][y+1]=side;
		default:
			break;
		}
	}

	void givePrevSide(int x,int y,int side,int dir)
	{
		switch(dir)
		{
		case LAY:
			if( y-1<1 || a[x][y-1]!=0 ) return;
			a[x][y-1]=side;
			break;
		case STAND:
			if(x-1<1 || a[x-1][y]!=0 ) return;
			a[x-1][y]=side;
			break;
		case MAIN:
			if( y-1<1 || x-1<1 || a[x-1][y-1]!=0 ) return;
			a[x-1][y-1]=side;
			break;
		case VICE:
			if(y-1<1 || x+1>len || a[x+1][y-1]!=0 ) return;
			a[x+1][y-1]=side;
		default:
			break;
		}
	}

	void resetNextSide(int x,int y,int side,int dir)
	{
		switch(dir)
		{
		case LAY:
			if( y+1<=len && a[x][y+1]==side) a[x][y+1]=0;
			break;
		case STAND:
			if(x+1<=len && a[x+1][y]==side) a[x+1][y]=0;
			break;
		case MAIN:
			if(x+1<=len && y+1<=len && a[x+1][y+1]==side) a[x+1][y+1]=0;
			break;
		case VICE:
			if(x-1>=1 && y+1<=len && a[x-1][y+1]==side) a[x-1][y+1]=0;
		default:
			break;
		}
	}

	void resetPrevSide(int x,int y,int side,int dir)
	{
		switch(dir)
		{
		case LAY:
			if(y-1>=1 && a[x][y-1]==side) a[x][y-1]=0;
			break;
		case STAND:
			if(x-1>=1&& a[x-1][y]==side) a[x-1][y]=0;
			break;
		case MAIN:
			if(x-1>=1 && y-1>=1 && a[x-1][y-1]==side) a[x-1][y-1]=0;
			break;
		case VICE:
			if(x+1<=len && y-1>=1 && a[x+1][y-1]==side) a[x+1][y-1]=0;
		default:
			break;
		}
	}

	int getNext(int x,int y,int dir)
	{
		int result=0;
		switch(dir)
		{
		case LAY:
			if(y+1>len) return -1;
			result=a[x][y+1];
			break;
		case STAND:
			if(x+1>len) return -1;
			result=a[x+1][y];
			break;
		case MAIN:
			if(x+1>len || y+1>len) return -1;
			result=a[x+1][y+1];
			break;
		case VICE:
			if(x-1<1 || y+1>len) return -1;
			result=a[x-1][y+1];
		default:
			break;
		}
		return result;
	}

	int getPrev(int x,int y,int dir)
	{
		int result=0;
		switch(dir)
		{
		case LAY:
			if(y-1<1) return -1;
			result=a[x][y-1];
			break;
		case STAND:
			if(x-1<1) return -1;
			result=a[x-1][y];
			break;
		case MAIN:
			if(x-1<1 || y-1<1) return -1;
			result=a[x-1][y-1];
			break;
		case VICE:
			if(x+1>len || y-1<1) return -1;
			result=a[x+1][y-1];
		default:
			break;
		}
		return result;
	}

	location getNextLocation(int x,int y,int dir)
	{
		location k(x,y);
		switch(dir)
		{
		case LAY:
			k.x=x;
			k.y=y+1;
			break;
		case STAND:
			k.x=x+1;
			k.y=y;
			break;
		case MAIN:
			k.x=x+1;
			k.y=y+1;
			break;
		case VICE:
			k.x=x-1;
			k.y=y+1;
		default:
			break;
		}
		return k;
	}

	location getPrevLocation(int x,int y,int dir)
	{
		location k(x,y);
		switch(dir)
		{
		case LAY:
			k.x=x;
			k.y=y-1;
			break;
		case STAND:
			k.x=x-1;
			k.y=y;
			break;
		case MAIN:
			k.x=x-1;
			k.y=y-1;
			break;
		case VICE:
			k.x=x+1;
			k.y=y-1;
		default:
			break;
		}
		return k;
	}

	int inLimits(int x,int y)
	{
		if(x<=len && x>=1 && y<=len && y>=1) return 1;
		return 0;
	}

	int inLimits(location lo)
	{
		if(lo.x<=len && lo.x>=1 && lo.y<=len && lo.y>=1) return 1;
		return 0;
	}

	int inLimits(location lo1,location lo2,location lo3,location lo4)
	{
		return (inLimits(lo1)&&inLimits(lo2)&&inLimits(lo3)&&inLimits(lo4));
	}

	int getValue(int x,int y)
	{
		return a[x][y];
	}

	int getValue(location lo)
	{
		return getValue(lo.x,lo.y);
	}

public:
//==========================最大棋子数
	int getMaxNum(int x,int y,int side)
	{
		int lay=left(x,y,side)+right(x,y,side)+1;
		int stand=up(x,y,side)+down(x,y,side)+1;
		int main=leftUp(x,y,side)+rightDown(x,y,side)+1;
		int vice=rightUp(x,y,side)+leftDown(x,y,side)+1;
		int max=lay;
		if(stand>max) max=stand;
		if(main>max) max=main;
		if(vice>max) max=vice;
		return max;
	}//最大棋子数

	
	//==========================棋子头尾
	
	location getHeadLocation(int x,int y,int side,int dir)//==========================棋子头尾
	{
		location b;
		switch(dir)
		{
		case LAY:
			b=getLayHead(x,y,side);
			break;
		case STAND:
			b=getStandHead(x,y,side);
			break;
		case MAIN:
			b=getMainHead(x,y,side);
			break;
		case VICE:
			b=getViceHead(x,y,side);
			break;
		default:
			break;
		}
		return b;
	}

	location getTailLocation(int x,int y,int side,int dir)
	{
		location b;
		switch(dir)
		{
		case LAY:
			b=getLayTail(x,y,side);
			break;
		case STAND:
			b=getStandTail(x,y,side);
			break;
		case MAIN:
			b=getMainTail(x,y,side);
			break;
		case VICE:
			b=getViceTail(x,y,side);
			break;
		default:
			break;
		}
		return b;
	}

	location getLayHead(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while(y>=1 && a[x][y]==side) y--;
		a[tmpX][tmpY]=tmp;
		return location(x,y+1);
	}

	location getStandHead(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while(  x>=1 && a[x][y]==side) x--;
		a[tmpX][tmpY]=tmp;
		return location(x+1,y);
	}

	location getMainHead(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while(x>=1 && y>=1 && a[x][y]==side) x--,y--;
		a[tmpX][tmpY]=tmp;
		return location(x+1,y+1);
	}

	location getViceHead(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while( x<=len && y>=1 && a[x][y]==side) x++,y--;
		a[tmpX][tmpY]=tmp;
		return location(x-1,y+1);
	}

	location getLayTail(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while( y<=len && a[x][y]==side) y++;
		a[tmpX][tmpY]=tmp;
		return location(x,y-1);
	}

	location getStandTail(int x,int y,int side)//<----fuckerrrrrrrrrrrrrrr
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[x][y]=side;
		
		while( x<=len && a[x][y]==side) x++;
		
		a[tmpX][tmpY]=tmp;
		return location(x-1,y);
	}

	location getMainTail(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while(x<=len && y<=len && a[x][y]==side ) x++,y++;
		a[tmpX][tmpY]=tmp;
		return location(x-1,y-1);
	}

	location getViceTail(int x,int y,int side)
	{
		int tmpX=x;
		int tmpY=y;
		int tmp=a[tmpX][tmpY];
		a[tmpX][tmpY]=side;
		while(x>=1 && y<=len && a[x][y]==side ) x--,y++;
		a[tmpX][tmpY]=tmp;
		return location(x+1,y-1);
	}

	int getHeadTail(int x,int y,int side,int dir)
	{
		int ht=0;
		switch(dir)
		{
		case LAY:
			ht=layHeadTail(x,y,side);
			break;
		case STAND:
			ht=standHeadTail(x,y,side);
			break;
		case MAIN:
			ht=mainHeadTail(x,y,side);
			break;
		case VICE:
			ht=viceHeadTail(x,y,side);
			break;
		default:
			ht=0;
			break;
		}
		return ht;
	}

	int layHeadTail(int x,int y,int side)
	{
		int L=0;
		int R=0;

		int tmpX=x;
		int tmpY=y;
		int tmpV=a[x][y];
		a[x][y]=side;

		int tmp=y;
		while(a[x][y]==side)
		{
			y--;
			if(y<1) break;
		}
		if(y>=1 && a[x][y]==0 ) L=0;
		else L=1;

		while(a[x][tmp]==side)
		{
			 tmp++;
			 if(tmp>len) break;
		}
		if(tmp<=len && a[x][tmp]==0 ) R=0;
		else R=1;

		a[tmpX][tmpY]=tmpV;

		return getLR(L,R);
	}

	int standHeadTail(int x,int y,int side)
	{
		int Up=0;
		int Down=0;

		int tmpX=x;
		int tmpY=y;
		int tmpV=a[x][y];
		a[x][y]=side;

		int tmp=x;
		while(a[x][y]==side)
		{
			 x--;
			 if(x<1) break;
		}
		if( x>=1 && a[x][y]==0) Up=0;
		else Up=1;

		while(a[tmp][y]==side) 
		{
			tmp++;
			if(tmp>len) break;
		}
		if( tmp<=len && a[tmp][y]==0) Down=0;
		else Down=1;

		a[tmpX][tmpY]=tmpV;

		return getLR(Up,Down);
	}

	int mainHeadTail(int x,int y,int side)
	{
		int L=0;
		int R=0;
		int tmpX=x;
		int tmpY=y;

		int tmpVX=x;
		int tmpVY=y;
		int tmpV=a[x][y];
		a[x][y]=side;

		while(a[x][y]==side)
		{
			x--;
			y--;
			if(x<1 || y<1) break;
		}
		if(x>=1 && y>=1 && a[x][y]==0) L=0;
		else L=1;

		while(a[tmpX][tmpY]==side)
		{
			tmpX++;
			tmpY++;
			if(tmpX>len || tmpY>len) break;
		}
		if(tmpX<=len && tmpY<=len && a[tmpX][tmpY]==0) R=0;
		else R=1;

		a[tmpVX][tmpVY]=tmpV;
		
		return getLR(L,R);
	}

	int viceHeadTail(int x,int y,int side)
	{
		int L=0;
		int R=0;
		int tmpX=x;
		int tmpY=y;

		int tmpVX=x;
		int tmpVY=y;
		int tmpV=a[x][y];
		a[x][y]=side;

		while(a[x][y]==side)
		{
			x++;
			y--;
			if(x>len || y<1) break;
		}
		if(x<=len && y>=1 && a[x][y]==0 ) L=0;
		else L=1;

		while(a[tmpX][tmpY]==side)
		{
			tmpX--;
			tmpY++;
			if(tmpX<1 || tmpY>len) break;
		}
		if(tmpX>=1 && tmpY<=len && a[tmpX][tmpY]==0 ) R=0;
		else R=1;

		a[tmpVX][tmpVY]=tmpV;
		
		return getLR(L,R);	
	}

	int getLR(int L,int R)
	{
		if(L==0 && R==0) return LL;
		if(L==1 && R==1) return HH;
		if(L==0 && R==1) return LH;
		if(L==1 && R==0) return HL;
		return 0;
	}

//===========================棋子个数
	int getNum(int x,int y,int side,int dir)
	{
		int num=0;
		switch(dir)
		{
		case LAY:
			num=layNum(x,y,side);
			break;
		case STAND:
			num=standNum(x,y,side);
			break;
		case MAIN:
			num=mainNum(x,y,side);
			break;
		case VICE:
			num=viceNum(x,y,side);
			break;
		default:
			break;
		}
		return num;
	}

	int layNum(int x,int y,int side)
	{
		return left(x,y,side)+right(x,y,side)+1;
	}

	int standNum(int x,int y,int side)
	{
		return up(x,y,side)+down(x,y,side)+1;
	}

	int mainNum(int x,int y,int side)
	{
		return leftUp(x,y,side)+rightDown(x,y,side)+1;
	}

	int viceNum(int x,int y,int side)
	{
		return rightUp(x,y,side)+leftDown(x,y,side)+1;
	}

	int up(int x,int y,int side)
	{
		int flag=0;
		if(x==1)
			return 0;
		x--;
		while(x>=1 && a[x][y]==side)
		{
			flag++;
			x--;
		}
		return flag;
	}

	int down(int x,int y,int side)
	{
		int flag=0;
		if(x==len)
			return 0;
		x++;
		while(x<=len && a[x][y]==side)
		{
			flag++;
			x++;
		}
		return flag;
	}

	int left(int x,int y,int side)
	{
		int flag=0;
		if(y==1)
			return 0;
		y--;
		while(y>=1 && a[x][y]==side)
		{
			flag++;
			y--;
		}
		return flag;
	}

	int right(int x,int y,int side)
	{
		int flag=0;
		if(y==len)
			return 0;
		y++;
		while(y<=len && a[x][y]==side)
		{
			flag++;
			y++;
		}
		return flag;
	}

	int leftUp(int x,int y,int side)
	{
		int flag=0;
		if(x==1 || y==1)
			return 0;
		x--;
		y--;
		while(x>=1 && y>=1 && a[x][y]==side)
		{
			flag++;
			x--;
			y--;
		}
		return flag;
	}

	int leftDown(int x,int y,int side)
	{
		int flag=0;
		if(x==len || y==1)
			return 0;
		x++;
		y--;
		while(x<=len && y>=1 && a[x][y]==side)
		{
			flag++;
			x++;
			y--;
		}
		return flag;
	}

	int rightUp(int x,int y,int side)
	{
		int flag=0;
		if(x==1 || y==len)
			return 0;
		x--;
		y++;
		while(x>=1 && y<=len && a[x][y]==side)
		{
			flag++;
			x--;
			y++;
		}
		return flag;
	}

	int rightDown(int x,int y,int side)
	{
		int flag=0;
		if(x==len || y==len)
			return 0;
		x++;
		y++;
		while(x<=len && y<=len && a[x][y]==side)
		{
			flag++;
			x++;
			y++;
		}
		return flag;
	}
};

int main()
{
	Array a=Array();
	int x,y;
	//a.outputMap();
	while(!a.win())
	{
		a.AI();
		//system("cls");
		a.outputMap();
		cin>>x>>y;
		a.putChess(x,y);
		//system("cls");
		a.outputMap();
	}
	

	

	//cout<<a.linkChaSleepTwo(x,y,1,MAIN)<<endl;

	/*cout<<a.getHeadTail(x,y,1,LAY)<<endl;
	cout<<a.getHeadTail(x,y,1,STAND)<<endl;
	cout<<a.getHeadTail(x,y,1,MAIN)<<endl;
	cout<<a.getHeadTail(x,y,1,VICE)<<endl;*/

	//cout<<a.linkLiveTwo(x,y,1,LAY)<<endl;
	//cout<<a.jumpLiveTwo(x,y,1,LAY)<<endl;
	//cout<<a.bigJumpLiveTwo(x,y,1,LAY)<<endl;

	//cout<<a.getHeadTail(x,y,1,MAIN)<<endl;	
	/*cout<<a.getHeadLocation(x,y,1,LAY).x<<" "<<a.getHeadLocation(x,y,1,LAY).y<<endl;
	cout<<a.getHeadLocation(x,y,1,STAND).x<<" "<<a.getHeadLocation(x,y,1,STAND).y<<endl;
	cout<<a.getHeadLocation(x,y,1,MAIN).x<<" "<<a.getHeadLocation(x,y,1,MAIN).y<<endl;
	cout<<a.getHeadLocation(x,y,1,VICE).x<<" "<<a.getHeadLocation(x,y,1,VICE).y<<endl;

	cout<<a.getTailLocation(x,y,1,LAY).x<<" "<<a.getTailLocation(x,y,1,LAY).y<<endl;
	cout<<a.getTailLocation(x,y,1,STAND).x<<" "<<a.getTailLocation(x,y,1,STAND).y<<endl;
	cout<<a.getTailLocation(x,y,1,MAIN).x<<" "<<a.getTailLocation(x,y,1,MAIN).y<<endl;
	cout<<a.getTailLocation(x,y,1,VICE).x<<" "<<a.getTailLocation(x,y,1,VICE).y<<endl;*/

	//cout<<a.getMax(x,y,0)<<endl;
	

	system("pause");
	return 0;
}