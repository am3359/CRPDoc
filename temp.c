u32 decodeCmd(u8 buf[],u8 len,u8 c[ ][8],u8 l[])
{
	//u8 c[8][8],l[8];
	u32 result;
    u8 i,j,k,steps;
	//1 2 3 4[V010;S01cpppp;P01ktttt;R01ctttt;T01b;D:yymmdd;N:hhmmss]
    //1 2 3 4(P0130020)W0171207;
	result=0;//无效数据
    steps=0;
    for(i=0;i<len;i++)
    {
        switch(steps)
        {
			case 0:
			//找起始位[或(
				if(buf[i] == '[')
				{
					j=0;//单个命令长
					k=0;//多少个命令
					steps=1;
				}
				else if(buf[i] == '(')
				{
					j=0;//单个命令长
					k=0;//多少个命令
					steps=2;
				}
				break;
			case 1:
			//找分隔符;或结束符]
				if(buf[i]==';')
				{
					if(j>=4)//两个;间隔小于最小指令长度丢弃
					{
						l[k]=j;
						k++;//多少个命令
						if(k>=8)
						{
							//超过8条命令
							result=0;//无效数据
							steps=3;
						}
					}
					j=0;//单个命令长
				}
				else if(buf[i]==']')
				{
					if(j>=4)//两个;间隔小于最小指令长度丢弃
					{
						l[k]=j;
						k++;
					}
					result=1;
					steps=3;
				}
				else if((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
				{
					c[k][j]=buf[i];
					j++;
				}
				break;
			case 2:
			//找分隔符;或结束符]
				if(buf[i]==';')
				{
					if(j>=4)//两个;间隔小于最小指令长度丢弃
					{
						l[k]=j;
						k++;//多少个命令
						if(k>=8)
						{
							//超过8条命令
							result=0;//无效数据
							steps=3;
						}
					}
					j=0;//单个命令长
				}
				else if(buf[i]==')')
				{
					if(j>=4)//两个;间隔小于最小指令长度丢弃
					{
						l[k]=j;
						k++;
					}
					result=2;
					steps=3;
				}
				else if((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
				{
					c[k][j]=buf[i];
					j++;
				}
				break;
			case 3:
				//等待
				break;
        }
    }
}