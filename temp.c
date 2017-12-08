u8 buffer1[]="1 2 3 4[V010;;;;cpppp;P01ktttt;R;;;;;tt;T01b;D0yymmdd;N0hh;mss]";
u8 buffer2[]="1 2 3 4(P0130020;;;W01 1207;;;;;;;;bcde)";
u8 buffer3[]="1 2 3 4(V010;S01cpppp;P01ktttt;R01!tttt;T01b;D0yymmdd;N0hhmmss)";
u8 cc[8][2];
u8 num;
u32 res;

u32 decodeCmd(u8 buf[],u8 len,u8 c[ ][2],u8 *n)
{
    //u8 c[8][8],l[8];
    u32 result;
    u8 i,j,k,steps;
    u8 valid,end;
    //1 2 3 4[V010;S01cpppp;P01ktttt;R01ctttt;T01b;D0yymmdd;N0hhmmss]
    //1 2 3 4(P0130020;;;W0171207;;;;;;;abcde)
    result=0;//无效数据
    steps=0;
    for(i=0;i<len;i++)
    {
        switch(steps)
        {
            case 0:
            //找起始位[或(
                j=0;//单个命令长
                k=0;//多少个命令
                c[0][0]=i+1;//命令位置
                c[0][1]=0;//命令长度
                if(buf[i] == '[')
                {
                    end=']';//保存结束符
                    valid=1;
                    steps=1;
                }
                else if(buf[i] == '(')
                {
                    end=')';//保存结束符
                    valid=1;
                    steps=1;
                }
                break;
            case 1:
            //找分隔符;或结束符]
                if(buf[i]==';')
                {
                    if((j>=4)&&(valid))//两个;间隔小于最小指令长度丢弃
                    {
                        c[k][1]=j;
                        k++;//多少个命令
                        if(k>=8)
                        {//超过8条命令
                            result=0;//无效数据
                            steps=2;
                        }
                    }
                    c[k][0]=i+1;
                    c[k][1]=0;
                    j=0;//单个命令长
                    valid=1;
                }
                else if(buf[i]==end)
                {
                    if((j>=4)&&(valid))//两个;间隔小于最小指令长度丢弃
                    {
                        c[k][1]=j;
                        k++;
                    }
                    if(end == ']')
                    {
                        result=1;
                    }
                    else if(end == ')')
                    {
                        result=2;
                    }
                    *n=k;
                    steps=3;
                }
                else if((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
                {
                    j++;
                }
                else
                {//出现字母和数字之外的字符都会使当前命令无效
                    valid=0;
                }
                break;
            default:
                //等待
                break;
        }
    }
    return result;
}